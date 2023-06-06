#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/list.h> 
#include <linux/time64.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/random.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");

// Global data

#define BUF_LEN 100000
#define NAN_LENGTH 9000000000
#define CLEAN_TIME  10
#define SWITCH_TIME  2
#define ADD_TIME  1 
#define REMOVE_TIME  1
#define NUM_TABLES  4
#define NUM_STOOLS  8
#define ENTRY_SIZE 500
#define ENTRY_NAME "add"
#define PERMS 0644
#define PARENT NULL

#define FRESHMAN 0
#define SOPHOMORE 1
#define JUNIOR 2
#define SENIOR 3
#define PROFESSOR 4

// create a proc entry
static struct proc_dir_entry* proc_entry;

// static struct proc_ops pops;
static char msg[BUF_LEN];
static int procfs_buf_len;
static int peopleServed = 0;
static int occupancy = 0;

// Mutexes
static struct mutex queue_lock;
static struct mutex table_lock;

// Time vars
static struct timespec64 openTime;
static struct timespec64 checkTime;

int globalCurrentTable = 1;

enum stoolStates{F, O, J, S, P, C, D}; // Freshman, Sophmores, Juniors, Seniors, Profs, Clean, Dirty
// EQN for waiting: (state  value + 1) * 5 seconds to finish their drink(s)
static enum barStates{OPEN, CLOSING, CLOSED}stateOfBar; // states of the bar
static enum waiterStates{OFFLINE, IDLE, LOADING, CLEANING, MOVING}waiterState;

// waiter thread
static struct task_struct *waiterThread = NULL;

void delete_customers(void);

// Customer struct object
typedef struct Customer
{
	struct timespec64 start_time;
	struct timespec64 target_time;
	int groupId; // needed for when a group is in queue / to sit as a group
	int groupSize;
	int startSeat;
	enum stoolStates type;
	char groupType;
	struct list_head list; 
} customer;

// queue list
struct 
{
	int groupCount;
	int customerCount;
	struct list_head list;
} customers;

// struct stool that holds custometers
struct stool
{
	enum stoolStates currentState; // default it to C
	struct Customer* patron; // pointer to an existing patron 
};

// table struct that holds 8 stools
struct table
{
	struct stool stools[NUM_STOOLS];
};

// bar struct that holds tables
static struct bar
{
	struct table allTables[NUM_TABLES];
} mainBar; // Note: mainBar is the var name for our entire bar


// proc read -- displays the whole program
static ssize_t procfile_read(struct file* file, char * ubuf, size_t count, loff_t *ppos)
{
	int i = 0;
	int j = 0;
	int t = 0;
	int s = 0;
	int k = 0;
	int displayNum;
	int dummyL;
	int cntF = 0;
    int cntO = 0;
    int cntJ = 0;
    int cntS = 0;
    int cntP = 0;
	customer *c;
	struct list_head *temp;
	
	//strcpy(msg,"");

	procfs_buf_len = strlen(msg);
	

	if (*ppos > 0 || count < procfs_buf_len)
		return 0;

	if (copy_to_user(ubuf, msg, procfs_buf_len))
		return -EFAULT;
	

	// lock
	ktime_get_real_ts64(&checkTime);
	dummyL = mutex_lock_interruptible(&table_lock);
	dummyL = mutex_lock_interruptible(&queue_lock);

	*ppos = procfs_buf_len;

	procfs_buf_len = sprintf(msg, "Waiter State: ");
	if (waiterState == OFFLINE)
		procfs_buf_len += sprintf(msg+strlen(msg), "OFFLINE\n");
	else if (waiterState == IDLE)
		procfs_buf_len += sprintf(msg+strlen(msg), "IDLE\n");
	else if (waiterState == LOADING)
		procfs_buf_len += sprintf(msg+strlen(msg), "LOADING\n");
	else if (waiterState == CLEANING)
		procfs_buf_len += sprintf(msg+strlen(msg), "CLEANING\n");
	else if (waiterState == MOVING)
		procfs_buf_len += sprintf(msg+strlen(msg), "MOVING\n");

	if(globalCurrentTable == 1)
		displayNum = 4;
	else if (globalCurrentTable == 2)
		displayNum = 3;
	else if (globalCurrentTable == 3)
		displayNum = 2;
	else
		displayNum = 1;	

	procfs_buf_len += sprintf(msg+strlen(msg), "Current Table: %d\n", displayNum); // (globalCurrentTable*-1)%4);
	procfs_buf_len += sprintf(msg+strlen(msg), "Elapsed Time: %lld\n", checkTime.tv_sec - openTime.tv_sec);
	procfs_buf_len += sprintf(msg+strlen(msg), "Current Occupancy: %d\n", occupancy);

	procfs_buf_len += sprintf(msg+strlen(msg), "Bar Status: ");
	
	while (t < NUM_TABLES)
    {
        s = 0;
        while (s < NUM_STOOLS)
        {
            if (mainBar.allTables[t].stools[s].currentState == F)
                ++cntF;
            else if (mainBar.allTables[t].stools[s].currentState == O)
                ++cntO;
            else if (mainBar.allTables[t].stools[s].currentState == J)
                ++cntJ;
            else if (mainBar.allTables[t].stools[s].currentState == S)
                ++cntS;
            else if (mainBar.allTables[t].stools[s].currentState == P)
                ++cntP;
            ++s;
        }
        ++t;
    }

    if (cntF != 0)
	{
        procfs_buf_len += sprintf(msg+strlen(msg), "%d F. ", cntF);
	}
    if (cntO != 0)
	{
        procfs_buf_len += sprintf(msg+strlen(msg), "%d O. ", cntO);
	}
    if (cntJ != 0)
	{
        procfs_buf_len += sprintf(msg+strlen(msg), "%d J. ", cntJ);
	}
    if (cntS != 0)
	{
        procfs_buf_len += sprintf(msg+strlen(msg), "%d S. ", cntS);
	}
    if (cntP != 0)
	{
        procfs_buf_len += sprintf(msg+strlen(msg), "%d P. ", cntP);
	}

	procfs_buf_len += sprintf(msg+strlen(msg), "\nNumber of Customers Waiting: %d\n", customers.customerCount);
	procfs_buf_len += sprintf(msg+strlen(msg), "Number of Groups Waiting: %d\n", customers.groupCount);
	procfs_buf_len += sprintf(msg+strlen(msg), "Contents of Queue:\n");

	list_for_each(temp, &customers.list)
	{
		c = list_entry(temp, customer, list);
		k = 0;
		
		while (k < c->groupSize)
		{
			procfs_buf_len += sprintf(msg+strlen(msg), "%c ", c->groupType); // %c
			++k;
		}
		
		procfs_buf_len += sprintf(msg+strlen(msg), "(group id: %d)\n", c->groupId);
	}
	
	

	procfs_buf_len += sprintf(msg+strlen(msg), "Number of Customers Serviced: %d\n", peopleServed);


	while (i < NUM_TABLES)
	{
		j = 0;

		procfs_buf_len += sprintf(msg+strlen(msg), "[");

		if (i == globalCurrentTable - 1)
			procfs_buf_len += sprintf(msg+strlen(msg), "*");
		else		
			procfs_buf_len += sprintf(msg+strlen(msg), " ");
		
		procfs_buf_len += sprintf(msg+strlen(msg), "]");

		//procfs_buf_len += sprintf(msg+strlen(msg), "Table %d: ", i+1);

		if (i + 1 == 1)
		{
			procfs_buf_len += sprintf(msg+strlen(msg), " Table %d: ", 4);
		}
		if (i + 1 == 2)
		{
	   	        procfs_buf_len += sprintf(msg+strlen(msg), " Table %d: ", 3);
		}
		if (i + 1 == 3)
		{
			procfs_buf_len += sprintf(msg+strlen(msg), " Table %d: ", 2);
		}
		if (i + 1 == 4)
		{
			procfs_buf_len += sprintf(msg+strlen(msg), " Table %d: ", 1);
		}

		while (j < NUM_STOOLS)
		{
			if (mainBar.allTables[i].stools[j].currentState == F)
				procfs_buf_len += sprintf(msg+strlen(msg), "F ");
			else if (mainBar.allTables[i].stools[j].currentState == O)
				procfs_buf_len += sprintf(msg+strlen(msg), "O ");
			else if (mainBar.allTables[i].stools[j].currentState == J)
				procfs_buf_len += sprintf(msg+strlen(msg), "J ");
			else if (mainBar.allTables[i].stools[j].currentState == S)
				procfs_buf_len += sprintf(msg+strlen(msg), "S ");
			else if (mainBar.allTables[i].stools[j].currentState == P)
				procfs_buf_len += sprintf(msg+strlen(msg), "P ");
			else if (mainBar.allTables[i].stools[j].currentState == C)
				procfs_buf_len += sprintf(msg+strlen(msg), "C ");
			else if (mainBar.allTables[i].stools[j].currentState == D)
				procfs_buf_len += sprintf(msg+strlen(msg), "D ");

			j++;
		}
		i++;
		
		procfs_buf_len += sprintf(msg+strlen(msg), "\n");
	}
	
	procfs_buf_len += sprintf(msg+strlen(msg), "                                                      "); // 40 spaces for the buffer eating gods. RIP human sacrifices // Extra 10 to a total fo 50, the gods hunger for more blood, 4 more for 54

	*ppos = procfs_buf_len;

	mutex_unlock(&queue_lock);
	mutex_unlock(&table_lock);


	return procfs_buf_len;
}

static struct proc_ops procfile_fops = {
	.proc_read = procfile_read,
};

// helper function that checks if a table is empty or not
int isEmpty(int tblCount) // returns 1 if the given table has NO patrons
{
	int empty = 0;
	int i = 0;
	while(i < NUM_STOOLS)
	{
		if(mainBar.allTables[tblCount].stools[i].currentState == C || mainBar.allTables[tblCount].stools[i].currentState == D)
			empty = 1;
		else
			return 0;
		++i;
	}
	return empty;
}

// helper function that returns the amount of clean tables
int cleanCount(int tblCount)
{
	int i = 0;
	int count = 0;
	while(i < NUM_STOOLS)
	{
		if(mainBar.allTables[tblCount].stools[i].currentState == C)
			++count;
		++i;
	}
	return count;
}

// helper function that returns the amount of dirty tables
int dirtyCount(int tblCount)
{
	int i = 0;
	int count = 0;
	while(i < NUM_STOOLS)
	{
		if(mainBar.allTables[tblCount].stools[i].currentState == D)
			++count;
		++i;
	}
	return  count;
}

// checks whether the table can be cleaned or not
int canCleanTable(int tblCount)
{
	int canClean = 0;
	canClean = isEmpty(tblCount);
	canClean = (dirtyCount(tblCount) >= 1);
	return canClean;
}

// function that cleans the table
int cleanTable(int tblCount)
{
	int canClean = canCleanTable(tblCount);
	int cleanCnt = cleanCount(tblCount);
	int shouldClean = 0;
	int lockRet;
	int i = 0;
	int nextTable = tblCount;
	customer* c;
	if(canClean)
	{
		// conditions: if there are not enough clean spaces for the
		// next group in the queue
		// OR all are dirty
		// queue is empty
		// if next table is empty AND queue not empty AND queue cant fit current table, move on
		
		// if queue is empty then clean
		if(dirtyCount(tblCount) == NUM_STOOLS)
			shouldClean = 1;
		if(customers.groupCount == 0)
			shouldClean = 1;
		else
		{
			c = list_first_entry(&customers.list, customer, list);
			if(c->groupSize >  cleanCnt)
			{
				shouldClean = 1;
				nextTable = (nextTable + 1) % NUM_TABLES;
				if(c->groupSize <= cleanCount(nextTable))
					shouldClean = 0;
			}
			
		}

		if(shouldClean)	
		{
			lockRet = mutex_lock_interruptible(&table_lock);
			waiterState = CLEANING;
			mutex_unlock(&table_lock);
			

			ssleep(CLEAN_TIME);
			// change waiter state to cleaning or whatever
			lockRet = mutex_lock_interruptible(&table_lock);
			while(i < NUM_STOOLS)
			{
				mainBar.allTables[tblCount].stools[i].currentState = C;
				++i;
			}
			mutex_unlock(&table_lock);
			if(lockRet !=0)
				printk(KERN_WARNING "Lock interrupted in cleanTable");
			return 1;
		}
	}
	return 0;
}

// remove customers / groups from table after their drink time is up
void removeFromTable(int tblCount) // Only god knows how this works now
{
	int i = 0;
	int j = 0;
	int k = 0;
	int lockRet;
	struct timespec64 currentTime;
	struct Customer* tempCustomer = NULL;
	//printk("MaASDbvASNDVASHGVDASCGDCASGDst the forbidden if");
	while(i < NUM_STOOLS)
	{
		if(mainBar.allTables[tblCount].stools[i].currentState != C && mainBar.allTables[tblCount].stools[i].currentState != D)
		{
			// If table has a person, check times
			//printk("Stool is not empty");
			tempCustomer = mainBar.allTables[tblCount].stools[i].patron;

			ktime_get_real_ts64(&currentTime);
			// current - start > target
			if(tempCustomer != NULL && ((currentTime.tv_sec - tempCustomer->start_time.tv_sec) >= ((tempCustomer->type+1)*5)))
			{
				//printk("Made it past the forbidden if");
				// remove person
				//de-allocate?
				j = tempCustomer->startSeat;
				lockRet = mutex_lock_interruptible(&table_lock);
                waiterState = LOADING;
				mutex_unlock(&table_lock);
				ssleep(REMOVE_TIME);
				k = tempCustomer->startSeat;
				lockRet = mutex_lock_interruptible(&table_lock);
				lockRet = mutex_lock_interruptible(&queue_lock);
				while(k < j + tempCustomer->groupSize) // if breaks, set to strictly less than
				{
					mainBar.allTables[tblCount].stools[k].currentState = D;
					++k;
				}
				//printk("Survived the loop");
				peopleServed += tempCustomer->groupSize;
				occupancy -= tempCustomer->groupSize;
				kfree(tempCustomer);
				mutex_unlock(&queue_lock);
				mutex_unlock(&table_lock);
				if(lockRet != 0)
					printk(KERN_WARNING "Lock interrupted in removeFromTable");	
			}
		}
		++i;
	}
}

// seat a group into a table
void seatGroup(int tblCount, int firstCall)
{
	// We need to get partySize from the first person in the queue, and loop form them to size
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	int isSeated = 0;
	int consecClean = 0;
	int partySize = -1;
	int openSeats = 0;
	int qRet;
	int tRet;
	struct Customer* temp;
	struct list_head* pos;
	customer* inListCust;

	if(customers.groupCount == 0) //If queue is empty, return
		return;


	tRet = mutex_lock_interruptible(&table_lock);
	qRet = mutex_lock_interruptible(&queue_lock);
	temp = list_first_entry(&customers.list, customer, list);
	if(temp == NULL)
		return;	
	

	partySize = temp->groupSize;
 
	openSeats = cleanCount(tblCount);


	if (partySize <= openSeats) // If there are enough clean seats in general loop thru seats
	{
		//printk(KERN_INFO "STEP ONE two ICHI");

		while(i < NUM_STOOLS && isSeated == 0)
		{
			//printk("in da loopdyloopoooopopopoppoooopoop");
			if(mainBar.allTables[tblCount].stools[i].currentState == C && i != 7) // Keep track of how many consecutive clean seats there are
			{
				if(consecClean == 0) // Keep track of the first clean seat
					j = i;
				
				++consecClean;
			}
			else
			{
				//printk("when all ELSE failes");
				if (i == 7 && mainBar.allTables[tblCount].stools[i].currentState == C)
				{
					if(consecClean ==0)
						j = i;
					++consecClean;
				}
				if(partySize <= consecClean) // if they fit, go ahead and seat them
				{
					// Loop from j, to j + consecClean, seat them	
                    waiterState = LOADING;
					--customers.groupCount;
					list_del(&temp->list);
					customers.customerCount -= partySize;
					occupancy += partySize;

					if(firstCall == 1)
					{
		    			mutex_unlock(&queue_lock);
						mutex_unlock(&table_lock);

						ssleep(ADD_TIME);

						tRet = mutex_lock_interruptible(&table_lock);
        				qRet = mutex_lock_interruptible(&queue_lock);
					}

					k = j;
					while(k < j + partySize)		
					{
						//printk("WE IN DA PARTY LOOP");
						mainBar.allTables[tblCount].stools[k].currentState = temp->type;
						mainBar.allTables[tblCount].stools[k].patron = temp;
						++k;
					}	
					isSeated = 1;
					//printk("IsSeated to 1 adasd");
					// Set startSeat and start time
					temp->startSeat = j;
					ktime_get_real_ts64(&(temp->start_time)); 
					temp->target_time = temp->start_time;
					temp->target_time.tv_sec += (temp->type + 1) + 5;

					//Search thru the queue, if a
					if(customers.groupCount == 0) //If queue is empty, return
					{
						mutex_unlock(&queue_lock);
                                        	mutex_unlock(&table_lock);
						return;
					}
						// COME BACK FIND ME
					list_for_each(pos, &customers.list)
					{
						inListCust = list_entry(pos, customer, list);
						if (l < 50)
						{
							openSeats = cleanCount(tblCount);
							if(inListCust->groupSize <= openSeats)	
							{
								// Move inListCust to fron of queue
								
								list_del(&inListCust->list);
								list_add_tail(&inListCust->list, &(list_first_entry(&customers.list, customer, list))->list);

								mutex_unlock(&queue_lock);
        							mutex_unlock(&table_lock);
							        if(tRet !=0)
							                printk(KERN_WARNING "Lock interrupted in seatgroup : table lock");
							        if(qRet !=0)
							                printk(KERN_WARNING "Lock interrupted for queue in seatGroup");
								//printk("RECURRRRRRR");
								seatGroup(tblCount, 0);
								
								return;
							}
						}
						else 
							break;

						++l;
						
					}
				}
				else
					if(consecClean != openSeats) // If they cant be seated, reset and keep looking
						consecClean = 0;
			}
			++i;
		}
	}
	mutex_unlock(&queue_lock);
	mutex_unlock(&table_lock);
	if(tRet !=0)
        	printk(KERN_WARNING "Lock interrupted in seatgroup : table lock");
        if(qRet !=0)
                printk(KERN_WARNING "Lock interrupted for queue in seatGroup");
}

// start the waiter thread
int runWaiter(void *data)
{

	int tableCount = 0;
	int lockRet;
	//int stoolCount = 0;

	// actions Done
	int cleaned = 0;

	//printk("Waiter thread is running");

	while(!kthread_should_stop())
	{
		cleaned = 0;
		// when we reach a new table ()table count++ % 4 every iteration
		// Remove people
		// check if can/should clean 
		removeFromTable(tableCount);
		cleaned = cleanTable(tableCount);
		// add people
		seatGroup(tableCount, 1);
		// move to next

		// move to next table
		lockRet = mutex_lock_interruptible(&table_lock);
                waiterState = MOVING;
                mutex_unlock(&table_lock);

		ssleep(SWITCH_TIME);
		lockRet = mutex_lock_interruptible(&table_lock);
		tableCount = (tableCount + 1) % NUM_TABLES;
		globalCurrentTable = tableCount + 1;
		mutex_unlock(&table_lock);
		if(lockRet !=0)
			printk(KERN_WARNING "Lock interrupted in runWaiter");
	}

	return 1;
}

// close the bar
static void closeBar(void)
{

	// once empty,
	int i = 0;
	int isDone = 0;
	int lockRet;
	int ret;

	//printk("in close bar ");
	lockRet = mutex_lock_interruptible(&queue_lock);
	//printk("About to delete customers");
	delete_customers();
	mutex_unlock(&queue_lock);	

	//printk("REACHED PAST DELEte CUSTOMERS");
	while(isDone < NUM_TABLES)
	{
		//printk("IN THE IS DONE LOOP");

		if(isEmpty(i) == 1 && cleanCount(i) == NUM_STOOLS)
			++isDone;
		if(i == 0)
		{
			isDone = 0;
			if(isEmpty(i) == 1 && cleanCount(i) == NUM_STOOLS)
                        	++isDone;
		}
		++i;
		i %= NUM_TABLES;
		ssleep(1);
		//printk("A sleepy boi");
	}
	//printk("Escaped cleaing duties");
	ret = kthread_stop(waiterThread);
	waiterThread = NULL;
	//printk("Murdered the waiter");

	lockRet = mutex_lock_interruptible(&table_lock);
	waiterState = OFFLINE;
   	mutex_unlock(&table_lock);

	//printk("I plead 'oopsies' your honor");

	if(ret != -EINTR)
		printk("Waiter thread has stopped\n");
}

// set up the bar - run the thread
static int setupBar(void)
{

	int lockRet;

	lockRet = mutex_lock_interruptible(&table_lock);
    	waiterState = IDLE;
    	mutex_unlock(&table_lock);

	waiterThread = kthread_run(runWaiter, NULL, "waiter");
	if(IS_ERR(waiterThread))
	{
		printk("ERROR: waiterThread_run\n");
		return PTR_ERR(waiterThread);
	}


	return 1 ;
}

int isValidType(int type)
{
	return (type == FRESHMAN || type == JUNIOR || type == SOPHOMORE ||
			type == SENIOR || type == PROFESSOR);
}

// add customers into the queue
int add_customer(int group_size, int t)
{
	customer *newCustomer;
    customer *currentCustomer;
	int randID;
	int isSeated = 0;
        struct list_head *temp;
	int lockRet;
	if ((group_size < 1 && group_size > 9) || !(isValidType(t)))
	{
		return 1;
	}
	newCustomer = kmalloc(sizeof(customer) * 1, __GFP_RECLAIM);
	if (newCustomer == NULL)
	{
		return -ENOMEM;
	}

	randID = (get_random_int() % INT_MAX) + 1; // get rand num [0, INT_MAX]

	switch (t)
	{
		case FRESHMAN:
			newCustomer->type = F;
			newCustomer->groupType = 'F';
			break;
		case SOPHOMORE:
			newCustomer->type = O;
			newCustomer->groupType = 'O';
			break;
		case JUNIOR:
			newCustomer->type = J;
			newCustomer->groupType = 'J';
			break;
		case SENIOR:
			newCustomer->type = S;
			newCustomer->groupType = 'S';
			break;
		case PROFESSOR:
			newCustomer->type = P;
			newCustomer->groupType = 'P';
			break;
	}
	newCustomer->groupId = randID;
	newCustomer->groupSize = group_size;

	lockRet = mutex_lock_interruptible(&queue_lock);


	
	 list_for_each(temp, &customers.list)
    {
        currentCustomer = list_entry(temp, customer, list);

        // check if new customer has a higher priority
        if (currentCustomer->type > newCustomer->type)
        {
            // add new customer before the current
            list_add_tail(&newCustomer->list, temp);
            isSeated = 1;
            break;
        }
		else if (currentCustomer->type == newCustomer->type)
        {
            // if new customer has the same priority as current
            // check for the size
            // the smaller group has higher priority
            if (currentCustomer->groupSize <= newCustomer->groupSize) // original is >
            {
                // add new customer with the smaller group size before current one
                list_add_tail(&newCustomer->list, temp);
                isSeated = 1;
                break;
            }
        }
    }

    // if the queue is empty or newCustomer has the lowest priority in the list
    if (isSeated == 0)
	{
        list_add_tail(&newCustomer->list, &customers.list);
	}
	customers.groupCount += 1;
	customers.customerCount += group_size;
	mutex_unlock(&queue_lock);

	//if lock ret != 0 then problems
	if(lockRet != 0)
		printk(KERN_WARNING "Queue lock interrupted in add_customer");

	return 0;
}

// delete customers from the queue
void delete_customers(void)
{
	struct list_head *temp;
	struct list_head *dummy; // me fr
	customer *c;

	list_for_each_safe(temp, dummy, &customers.list) 
	{
		c = list_entry(temp, customer, list);
		list_del(temp);
		kfree(c);
	}
	customers.groupCount = 0;
	customers.customerCount = 0;
}

/* Adding syscall functionaliy here*/
extern int (*STUB_initialize_bar)(void);
int my_initialize_bar(void){
	// return 0 on open success, -1 on error, 1 on already open
	int setupRet = 0;

	if(stateOfBar == CLOSED)
	{
		stateOfBar = OPEN;
		// CALL FUNCTIONS TO OPEN BAR
		setupRet = setupBar();
		if(setupRet != 1)
			return -1;
		return 0;
	}
	else if(stateOfBar == OPEN || stateOfBar == CLOSING)
	{
		return 1;
	}

	// if theres some error
	return -1;
}

extern int (*STUB_close_bar)(void);
int my_close_bar(void){
	// return 0 on close, 1 on closed already

	if(stateOfBar == OPEN)
	{
		stateOfBar = CLOSING;
		closeBar();

		stateOfBar = CLOSED;

		return 0;
	}
	else if (stateOfBar == CLOSING || stateOfBar == CLOSED) // check write up if its a dif return val
	{
		return 1;
	}
	return 0;
}

extern int (*STUB_customer_arrival)(int, int);
int my_customer_arrival(int number_of_customers, int type)
{
	int addCustomerRet = 0;
	// add garbage
	// Should return zero  on success, one or other error on fail
	
	// if bar status is CLOSED or CLOSING then return 1
	if(stateOfBar == CLOSED || stateOfBar == CLOSING)
		return 1;
	addCustomerRet = add_customer(number_of_customers, type); 
	
	return addCustomerRet;
}

// initialize the barstool
static int barstool_init(void)
{
	int i = 0;
	int j = 0;
	ktime_get_real_ts64(&checkTime);
        ktime_get_real_ts64(&openTime);
	proc_entry = proc_create("majorsbar", 0666, NULL, &procfile_fops);

	if (proc_entry == NULL)
		return -ENOMEM;
	

	STUB_customer_arrival = my_customer_arrival;
	STUB_initialize_bar = my_initialize_bar;
	STUB_close_bar = my_close_bar;
	stateOfBar = CLOSED;
	waiterState = OFFLINE;


	sprintf(msg, "Waiter State OFFLINE\n");

	while(i < NUM_TABLES) // doesnt need a lock nothing else will mess with this
        {
                j = 0;
                while(j < NUM_STOOLS)
                {
                        mainBar.allTables[i].stools[j].currentState = C;
                        ++j;
                }
                ++i;
        }

	mutex_init(&queue_lock);
	mutex_init(&table_lock);

	customers.groupCount = 0;
	INIT_LIST_HEAD(&customers.list);

	return 0;
}

// exit the barstool
static void barstool_exit(void)
{
	int  ret;
	proc_remove(proc_entry);

	if(waiterThread != NULL)
		ret = kthread_stop(waiterThread);

	STUB_customer_arrival = NULL;
	STUB_initialize_bar = NULL;
	STUB_close_bar = NULL;	

	mutex_destroy(&queue_lock);
	mutex_destroy(&table_lock);

	return;
}

module_init(barstool_init);
module_exit(barstool_exit);