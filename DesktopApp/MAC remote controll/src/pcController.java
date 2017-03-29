package dit948;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.RenderingHints;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.Timer;

public class pcController extends JPanel implements ActionListener, KeyListener {
Timer t = new Timer(5, this);
int xPosition = 190, yPosition = 250; 
int aPosition = 90, bPosition = 150;
int moveX =0, moveY =0;
public static final Color VERY_DARK_BLUE = new Color(0x00, 0x00, 0x80);
public static final Color VERY_DARK_GREEN = new Color(0x00, 0x80, 0x00);

public pcController() {
	t.start();
	addKeyListener(this);
	setFocusable(true);
	setFocusTraversalKeysEnabled(false);
}

public void paintComponent(Graphics g) {
	super.paintComponent(g);
	Graphics2D g2 = (Graphics2D) g;
	g2.setRenderingHint(RenderingHints.KEY_ANTIALIASING,
          RenderingHints.VALUE_ANTIALIAS_ON);
	g.setColor(VERY_DARK_GREEN);
	g.fillOval(aPosition, bPosition,400,400);
	g.setColor(VERY_DARK_BLUE);
	g.fillOval(xPosition, yPosition,200,200);

}

public void actionPerformed(ActionEvent e) {
if(xPosition < 0)
{
	moveX = 0;
	xPosition = 190;		
}

if(xPosition > 290)
{
	moveX = 0;
	xPosition = 190;		
}

if(yPosition < 0)
{
	moveY = 0;
	yPosition = 0;		
}

if(yPosition > 350)
{
	moveY = 0;
	yPosition = 250;		
}


xPosition += moveX;
yPosition += moveY;
repaint();
}

public void keyPressed(KeyEvent e) {
	int code = e.getKeyCode();
	
	if (code == KeyEvent.VK_S){
		moveY = 1;
		moveX = 0;
	}
	if (code == KeyEvent.VK_W){
		moveY = -1;
		moveX = 0;
	}
	if (code == KeyEvent.VK_A){
		moveY = 0;
		moveX = -1;
	}
	if (code == KeyEvent.VK_D){
		moveY = 0;
		moveX = 1;
		}
	}



public void keyTyped(KeyEvent e) {}
public void keyReleased(KeyEvent e) {
	moveY = yPosition; 
	moveX = xPosition; 
	
}
public static void main (String arge[]){

	JFrame f = new JFrame("PC Controller");
	pcController s = new pcController();
	f.add(s);
	f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
	f.setSize(600,800);
	f.setLocationRelativeTo(null); 
	f.setVisible(true);

}


} 

