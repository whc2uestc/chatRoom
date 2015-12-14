import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.net.*;
import java.io.*;
class ServerThread extends Thread
{
	ChatServer father; //声明Server的引用
	boolean flag=true;
	Socket sc;
	public ServerThread(ChatServer father,Socket sc)
	{//构造器
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
	
	JLabel jlPort=new JLabel("Port");//创建提示输入端口号标签
	JTextField jtfPort=new JTextField("9999");//用于输入端口号的文本框
	JButton jbStart=new JButton("Start");//创建"启动"按钮
	JPanel jps=new JPanel();//创建一个JPanel对象
	TextArea ta = new TextArea();
	JSplitPane jspz=new JSplitPane(JSplitPane.HORIZONTAL_SPLIT,ta,jps);//创建JSplitPane对象
	ServerSocket ss;//声明ServerSocket引用
	ServerThread st;//声明ServerThread引用
	boolean flag = false;;
	HashSet<Socket> hs = new HashSet<Socket>();
	public void run() {
		while(true){
			if(flag){
				try{
					System.out.println("run1");
					Socket sc=ss.accept();//等待客户端连接
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
		this.initialComponent();//初始化控件
		this.addListener();//为相应的控件注册事件监听器
		this.initialFrame();//初始化窗体
	}
	//初始化控件
	public void initialComponent()
	{
		jps.setLayout(null);//设为空布局
		jlPort.setBounds(20,20,50,20);
		jps.add(jlPort);//添加用于提示输入端口号的标签
		this.jtfPort.setBounds(85,20,60,20);
		jps.add(this.jtfPort);//添加用于输入端口号的文本框
		this.jbStart.setBounds(18,50,80,20);
		jps.add(this.jbStart);//添加"开始"按钮
	}
	//为相应的控件注册事件监听器
	public void addListener()
	{
		this.jbStart.addActionListener(this);//为"开始"按钮注册事件监听器
	}
	//初始化窗体
	public void initialFrame()
	{
		this.setTitle("ChatServer");//设置窗体标题
		this.add(jspz);//将JSplitPane添加到窗体中
		jspz.setDividerLocation(250);
		jspz.setDividerSize(4);//设置分割线的位置和宽度
		this.setBounds(20,20,420,320);
		this.setVisible(true);//设置可见性	
	}
	//按钮事件  启动和关闭服务器
	public void actionPerformed(ActionEvent e)
	{
			this.jbStart_event();
	}
	//启动服务器事件处理
	public void jbStart_event()
	{
		//单击"启动"按钮的业务处理代码
		int port=0;
		try
		{
			//获得用户输入的端口号，并转化为整型
			port=Integer.parseInt(this.jtfPort.getText().trim());
		}
		catch(Exception ee)
		{//端口号不是整数，给出提示信息
			JOptionPane.showMessageDialog(this,"端口号只能是整数","错误",
			                               JOptionPane.ERROR_MESSAGE);
			return;
		}
		if(port>65535||port<0)
		{//断口号不合法，给出提示信息
			JOptionPane.showMessageDialog(this,"端口号只能是0-65535的整数","错误",
			                               JOptionPane.ERROR_MESSAGE);
			return;
		}
		try
		{
			this.jbStart.setEnabled(false);//将开始按钮设为不可用
			this.jtfPort.setEnabled(false);//将用于输入端口号的文本框设为不可用
			ss=new ServerSocket(port);//创建ServerSocket对象
			flag = true;
			//给出服务器启动成功的提示信息
			JOptionPane.showMessageDialog(this,"Server Start OK","Hint",
			                               JOptionPane.INFORMATION_MESSAGE);
		}
		catch(Exception ee)
		{
			//给出服务器启动失败的提示信息
			JOptionPane.showMessageDialog(this,"Server Start Fail","Error",
			                               JOptionPane.ERROR_MESSAGE);
			this.jbStart.setEnabled(true);//将开始按钮设为可用
			this.jtfPort.setEnabled(true);//将用于输入端口号的文本框设为可用
		}
	}
	public static void main(String args[])
	{
		Thread t = new Thread(new ChatServer());
		t.start();
	}
}

