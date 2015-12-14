import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.*;
import java.io.*;
class ServerThread extends Thread
{
	ChatServer father; //����Server������
	boolean flag=true;
	Socket sc;
	public ServerThread(ChatServer father,Socket sc)
	{//������
		this.father=father;
		this.sc=sc;
	}
	public void run()
	{
		while(flag)
		{
			try
			{
				BufferedReader br = new BufferedReader(new InputStreamReader(sc.getInputStream()));
				while(true){
					String s = br.readLine();
					if(s==null) break;
					father.ta.append(s+"\n");
					Iterator<Socket> iterator=father.hs.iterator();
					while(iterator.hasNext()){
						Socket sto = iterator.next();
						if(!sto.equals(sc)){
							PrintWriter pw = new PrintWriter(sto.getOutputStream());
							pw.println(s);
							pw.flush();
						}	
					}
				}
			}
			catch(Exception e)
			{
				System.out.println("error"+e);
			}
		}
	}
}

public class ChatServer extends JFrame implements ActionListener, Runnable
{
	
	JLabel jlPort=new JLabel("Port");//������ʾ����˿ںű�ǩ
	JTextField jtfPort=new JTextField("9999");//��������˿ںŵ��ı���
	JButton jbStart=new JButton("Start");//����"����"��ť
	JPanel jps=new JPanel();//����һ��JPanel����
	TextArea ta = new TextArea();
	JSplitPane jspz=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,ta,jps);//����JSplitPane����
	ServerSocket ss;//����ServerSocket����
	ServerThread st;//����ServerThread����
	boolean flag = false;;
	HashSet<Socket> hs = new HashSet<Socket>();
	public void run() {
		while(true){
			if(flag){
				try{
					System.out.println("run1");
					Socket sc=ss.accept();//�ȴ��ͻ�������
					System.out.println("accepted");
					hs.add(sc);
					new ServerThread(this,sc).start();
				}catch(Exception e){
					System.out.println("run error"+e);
				}
			}
		}
    }
	public ChatServer()
	{
		this.initialComponent();//��ʼ���ؼ�
		this.addListener();//Ϊ��Ӧ�Ŀؼ�ע���¼�������
		this.initialFrame();//��ʼ������
	}
	//��ʼ���ؼ�
	public void initialComponent()
	{
		jps.setLayout(null);//��Ϊ�ղ���
		jlPort.setBounds(20,20,50,20);
		jps.add(jlPort);//���������ʾ����˿ںŵı�ǩ
		this.jtfPort.setBounds(85,20,60,20);
		jps.add(this.jtfPort);//�����������˿ںŵ��ı���
		this.jbStart.setBounds(18,50,80,20);
		jps.add(this.jbStart);//���"��ʼ"��ť
	}
	//Ϊ��Ӧ�Ŀؼ�ע���¼�������
	public void addListener()
	{
		this.jbStart.addActionListener(this);//Ϊ"��ʼ"��ťע���¼�������
	}
	//��ʼ������
	public void initialFrame()
	{
		this.setTitle("ChatServer");//���ô������
		this.add(jspz);//��JSplitPane��ӵ�������
		jspz.setDividerLocation(250);
		jspz.setDividerSize(4);//���÷ָ��ߵ�λ�úͿ��
		this.setBounds(20,20,420,320);
		this.setVisible(true);//���ÿɼ���	
	}
	//��ť�¼�  �����͹رշ�����
	public void actionPerformed(ActionEvent e)
	{
			this.jbStart_event();
	}
	//�����������¼�����
	public void jbStart_event()
	{
		//����"����"��ť��ҵ�������
		int port=0;
		try
		{
			//����û�����Ķ˿ںţ���ת��Ϊ����
			port=Integer.parseInt(this.jtfPort.getText().trim());
		}
		catch(Exception ee)
		{//�˿ںŲ���������������ʾ��Ϣ
			JOptionPane.showMessageDialog(this,"�˿ں�ֻ��������","����",
			                               JOptionPane.ERROR_MESSAGE);
			return;
		}
		if(port>65535||port<0)
		{//�ϿںŲ��Ϸ���������ʾ��Ϣ
			JOptionPane.showMessageDialog(this,"�˿ں�ֻ����0-65535������","����",
			                               JOptionPane.ERROR_MESSAGE);
			return;
		}
		try
		{
			this.jbStart.setEnabled(false);//����ʼ��ť��Ϊ������
			this.jtfPort.setEnabled(false);//����������˿ںŵ��ı�����Ϊ������
			ss=new ServerSocket(port);//����ServerSocket����
			flag = true;
			//���������������ɹ�����ʾ��Ϣ
			JOptionPane.showMessageDialog(this,"Server Start OK","Hint",
			                               JOptionPane.INFORMATION_MESSAGE);
		}
		catch(Exception ee)
		{
			//��������������ʧ�ܵ���ʾ��Ϣ
			JOptionPane.showMessageDialog(this,"Server Start Fail","Error",
			                               JOptionPane.ERROR_MESSAGE);
			this.jbStart.setEnabled(true);//����ʼ��ť��Ϊ����
			this.jtfPort.setEnabled(true);//����������˿ںŵ��ı�����Ϊ����
		}
	}
	public static void main(String args[])
	{
		Thread t = new Thread(new ChatServer());
		t.start();
	}
}

