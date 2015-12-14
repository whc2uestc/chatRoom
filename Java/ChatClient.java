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
	private TextArea ta;    //�ı���
	private JTextField ip;  //ip������
	private JTextField port;  //�˿�������
	private JButton ss_client; //Connect����
	private JTextField send_text; //��Ϣ�����ı���
	private JTextField name;	  //�û����ı���
	private JButton ss_send;      //���Ͱ�ť
	private Socket skt;           //Socket����
	public void keyReleased(KeyEvent f){}
	
	//ͨ������������Ϣ
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
				ta.append("Send fail��\n");
			}
	 	}
	}
	public void keyTyped(KeyEvent f){}

	// Runnable�ӿڵ�ʵ�ַ���������������Ϣ
	public void run()
	{
		try{
			BufferedReader br = new BufferedReader(new InputStreamReader(skt.getInputStream()));
			while(true){
				String s = br.readLine(); //��ȡ���ջ�����������
				if(s==null) break;
				ta.append(s + "\n");      //��ʾ���ı���
			}
		}
		catch(Exception e){
			e.printStackTrace();
		}		
	}
	//�����¼���Connect����Ϊ�������ӣ�Send����Ϊ������Ϣ
	public void actionPerformed(ActionEvent e)
	{
		if(e.getSource()==ss_client){
			doClient();//��������
		}
		if(e.getSource()==ss_send){
			doSend();//������Ϣ
		}

	}
	//�������ӣ�����ʵ�ַ���		
	public void doClient()
	{
		try{
			skt = new Socket(ip.getText(), Integer.parseInt(port.getText()));//��������
			JOptionPane.showMessageDialog(this,"Connect success!!","Hint",
                    JOptionPane.INFORMATION_MESSAGE);//��ʾ���ӳɹ���Ϣ
			new Thread(this).start();
		}
		catch(Exception e){
			ta.append("connect fail��\n");
		}		
	}
	//������Ϣ������ʵ�ַ���
	public void doSend()
	{
		
		try{
			PrintWriter pw = new PrintWriter(skt.getOutputStream());
			String s1=name.getText();
			String s = send_text.getText();
			
			if(s==null) return;
		    ta.append(s1+":"+s+"\n");
		    String s3=s1+":"+s;
			pw.println(s3);   //д�������
			pw.flush();       //����������������
			send_text.setText("");
		}
		catch(Exception e){
			ta.append("Send Fail��\n");
		}
	}

	//ChatClient���췽������ʼ��GUI
	public ChatClient()
	{
		//��ʼ��GUI
		Panel a1 = new Panel();
		Panel a2 = new Panel();
		Panel a3 = new Panel();
		this.setTitle("chatter");
		this.setBounds(200,200,600,500);
		Container cc = this.getContentPane();
		
		JPanel p1 = new JPanel();
		cc.add(p1, BorderLayout.NORTH);
		p1.setBackground(new Color(200,200,250));
		//����ı���
		ta = new TextArea();
		cc.add(ta, BorderLayout.CENTER);
		JPanel p2 = new JPanel();
		cc.add(p2, BorderLayout.SOUTH);
		p2.setBackground(new Color(250,150,200));
		ta.setForeground(new Color(0,0,250));
		JLabel p3 = new JLabel(new ImageIcon("2010.jpg"));
		cc.add(p3, BorderLayout.EAST);
		//���Name,IP,PORT�ı���
		p1.add(new JLabel("Name:"));
		name=new JTextField("",6);
		p1.add(name);
		name.setFont(new Font("����",Font.BOLD,14));
		p1.add(new JLabel("IP: "));
		ip = new JTextField("127.0.0.1", 15);
		p1.add(ip);
		p1.add(new JLabel("Port: "));
		port = new JTextField("9999", 4);
		p1.add(port);

		//���Connect��ť
		ss_client = new JButton("Connect");
		ss_client.setForeground(new Color(150,100,250));
		ss_client.setFont(new Font("����",Font.BOLD,14));
		p1.add(ss_client);

		//��ӷ����ı�����Send��ť
		p2.setLayout(new BorderLayout());
		p2.setBackground(new Color(200,200,250));
		send_text = new JTextField("");
		p2.add(send_text, BorderLayout.CENTER);
		ss_send = new JButton("Send");
		p2.add(ss_send, BorderLayout.EAST);
		
		//����¼�
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
