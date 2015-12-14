import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Font;
import java.awt.Panel;
import java.awt.TextArea;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ChatClient extends JFrame implements ActionListener, Runnable,KeyListener{
	private TextArea ta;    //文本区
	private JTextField ip;  //ip输入区
	private JTextField port;  //端口输入区
	private JButton ss_client; //Connect按键
	private JTextField send_text; //消息发送文本区
	private JTextField name;	  //用户名文本区
	private JButton ss_send;      //发送按钮
	private Socket skt;           //Socket对象
	public void keyReleased(KeyEvent f){}
	
	//通过键盘输入消息
	public void keyPressed(KeyEvent f){
		if(f.getKeyCode()== KeyEvent.VK_ENTER){
			try{
				PrintWriter pw = new PrintWriter(skt.getOutputStream());
				String s1=name.getText();
				String s = send_text.getText();
				
				if(s==null) return;
				ta.append(s1+":"+s+"\n");
				String s3=s1+":"+s;
				pw.println(s3);
				pw.flush();
				send_text.setText("");
			}
			catch(Exception e){
				ta.append("Send fail！\n");
			}
	 	}
	}
	public void keyTyped(KeyEvent f){}

	// Runnable接口的实现方法，用来接收消息
	public void run()
	{
		try{
			BufferedReader br = new BufferedReader(new InputStreamReader(skt.getInputStream()));
			while(true){
				String s = br.readLine(); //读取接收缓冲区的数据
				if(s==null) break;
				ta.append(s + "\n");      //显示在文本区
			}
		}
		catch(Exception e){
			e.printStackTrace();
		}		
	}
	//按键事件，Connect按键为发起连接，Send按键为发送消息
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource()==ss_client){
			doClient();//发起连接
		}
		if(e.getSource()==ss_send){
			doSend();//发送消息
		}

	}
	//发起连接，具体实现方法		
	public void doClient()
	{
		try{
			skt = new Socket(ip.getText(), Integer.parseInt(port.getText()));//发起连接
			JOptionPane.showMessageDialog(this,"Connect success!!","Hint",
                    JOptionPane.INFORMATION_MESSAGE);//提示连接成功消息
			new Thread(this).start();
		}
		catch(Exception e){
			ta.append("connect fail！\n");
		}		
	}
	//发送消息，具体实现方法
	public void doSend()
	{
		
		try{
			PrintWriter pw = new PrintWriter(skt.getOutputStream());
			String s1=name.getText();
			String s = send_text.getText();
			
			if(s==null) return;
		    ta.append(s1+":"+s+"\n");
		    String s3=s1+":"+s;
			pw.println(s3);   //写入输出流
			pw.flush();       //清空输出流，即发送
			send_text.setText("");
		}
		catch(Exception e){
			ta.append("Send Fail！\n");
		}
	}

	//ChatClient构造方法，初始化GUI
	public ChatClient()
	{
		//初始化GUI
		Panel a1 = new Panel();
		Panel a2 = new Panel();
		Panel a3 = new Panel();
		this.setTitle("chatter");
		this.setBounds(200,200,600,500);
		Container cc = this.getContentPane();
		
		JPanel p1 = new JPanel();
		cc.add(p1, BorderLayout.NORTH);
		p1.setBackground(new Color(200,200,250));
		//添加文本区
		ta = new TextArea();
		cc.add(ta, BorderLayout.CENTER);
		JPanel p2 = new JPanel();
		cc.add(p2, BorderLayout.SOUTH);
		p2.setBackground(new Color(250,150,200));
		ta.setForeground(new Color(0,0,250));
		JLabel p3 = new JLabel(new ImageIcon("2010.jpg"));
		cc.add(p3, BorderLayout.EAST);
		//添加Name,IP,PORT文本区
		p1.add(new JLabel("Name:"));
		name=new JTextField("",6);
		p1.add(name);
		name.setFont(new Font("宋体",Font.BOLD,14));
		p1.add(new JLabel("IP: "));
		ip = new JTextField("127.0.0.1", 15);
		p1.add(ip);
		p1.add(new JLabel("Port: "));
		port = new JTextField("9999", 4);
		p1.add(port);

		//添加Connect按钮
		ss_client = new JButton("Connect");
		ss_client.setForeground(new Color(150,100,250));
		ss_client.setFont(new Font("宋体",Font.BOLD,14));
		p1.add(ss_client);

		//添加发送文本区和Send按钮
		p2.setLayout(new BorderLayout());
		p2.setBackground(new Color(200,200,250));
		send_text = new JTextField("");
		p2.add(send_text, BorderLayout.CENTER);
		ss_send = new JButton("Send");
		p2.add(ss_send, BorderLayout.EAST);
		
		//添加事件
		send_text.addKeyListener(this);
		ss_client.addActionListener(this);		
		ss_send.addActionListener(this);		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);		
	}
	public static void main(String[] args)
	{
		new ChatClient().setVisible(true);
	}
	
}
