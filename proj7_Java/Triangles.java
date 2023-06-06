/*
 * Assignment #5
 * Due: 06/29/2022
 * Author: Evgeniya Kalashnikova
 * 
 * Triangle.java file contains:
 *  class Triangles with entry poit
 *  class TriangleJPanel
 * 
 * This program draws 5 triangle in random places (within a JPanel),
 * in random colors
 */

import javax.swing.JFrame;
import javax.swing.JPanel;
import java.awt.Color;
import java.awt.Graphics;
import java.util.Random;
import java.awt.geom.GeneralPath;
import java.awt.Graphics2D;

public class Triangles {

    public static void main(String[] args) {

        JFrame frame = new JFrame("Triangles"); // create a JFrame
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        TriangleJPanel panel = new TriangleJPanel();
        frame.add(panel); // add panel to frame
        frame.setBackground(Color.WHITE);
        frame.setSize(500, 500); // set initial size to 500 x 500
        frame.setVisible(true); // display frame

    }
}

// a panel with 5 triangles that are located in random places within
// this panel in random colors
class TriangleJPanel extends JPanel {

    public void paintComponent(Graphics g) {
        super.paintComponent(g); // call superclass's paintComponent
        Random random = new Random(); // get random number generator

        // draws and fills 5 triangles
        for (int i = 1; i <= 5; i++) {
            Graphics2D g2d = (Graphics2D) g;
            GeneralPath trg = new GeneralPath(); // create GeneralPath object

            // random starting / first point
            trg.moveTo(random.nextInt(getWidth() + 1),
                    random.nextInt(getHeight() + 1));
            // random second point
            trg.lineTo(random.nextInt(getWidth() + 1),
                    random.nextInt(getHeight() + 1));
            // random third point
            trg.lineTo(random.nextInt(getWidth() + 1),
                    random.nextInt(getHeight() + 1));
            // connect the third point to the first one
            trg.closePath();

            // set a random color for a shape (triangle)
            g2d.setColor(new Color(random.nextInt(256),
                    random.nextInt(256), random.nextInt(256)));
            // fill triangle with a random color
            g2d.fill(trg);
        }
    }
}
