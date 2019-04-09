using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 调用dll并读取图片
{
    public partial class MainWindow : Form
    {
        DataClass.MyMeans dataoperate = new 调用dll并读取图片.DataClass.MyMeans();
        DataSet ds;
        public MainWindow()
        {
            InitializeComponent();
            
        }
        private string openpathname = "C:\\opencv\\data_2.jpg";     		//定义绝对路径名，与算法处理(C++)的路径一致,此路径为初始路径

        //private string savepathname = string.Empty;

        private void MainWindow_Load(object sender, EventArgs e)
        {
            F_Login FrmLogin = new F_Login();                        //声时登录窗体，进行调用
            FrmLogin.Tag = 1;                                        //将登录窗体的Tag属性设为1，表示调用的是登录窗体
            FrmLogin.ShowDialog();
            FrmLogin.Dispose();
            /*当调用的是登录窗体时*/
            if (DataClass.MyMeans.Login_n == 1)
            {
                statusStrip2.Items[1].Text = DataClass.MyMeans.Login_Name;  //在状态栏显示当前登录的用户名
                dateTimePicker2.Format = DateTimePickerFormat.Time;
                dateTimePicker1.Format = DateTimePickerFormat.Custom;
                dateTimePicker1.CustomFormat = "MMMM dd , yyyy - dddd";
            }
            DataClass.MyMeans.Login_n = 3;                          //将公共变量设为3，便于控制登录窗体的关闭
            lviewBind();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            clrClass ClrClass = new clrClass();
            ClrClass.saveImage();
            MessageBox.Show("数据采集成功 ！");
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (DataClass.MyMeans.Login_Name == "裴成龙")
            {
                if (txtName.Text == string.Empty)
                {
                    MessageBox.Show("用户名不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    ds = dataoperate.getDataSet("select * from tb_Login where name='" + txtName.Text + "'", "tb_Login");
                    if (ds.Tables[0].Rows.Count > 0)
                    {
                        MessageBox.Show("该用户已经存在！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                    }
                    else
                    {
                        dataoperate.getsqlcom("insert into tb_Login (name,pass) values('" + txtName.Text + "','" + txtPwd.Text + "')");
                        lviewBind();
                        txtName.Text = txtPwd.Text = string.Empty;
                    }
                }
            }
            else
            {
                MessageBox.Show("您不是超级用户，没有权限执行此操作！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            if (DataClass.MyMeans.Login_Name == "裴成龙")
            {
                if (txtName.Text == string.Empty && txtPwd.Text == string.Empty)
                {
                    MessageBox.Show("用户名或密码不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    dataoperate.getsqlcom("update tb_Login set pass ='" + txtPwd.Text + "' where name='" + txtName.Text + "'");
                    lviewBind();
                    txtName.Text = txtPwd.Text = string.Empty;
                }
            }
            else
            {
                if (txtName.Text == string.Empty && txtPwd.Text == string.Empty)
                {
                    MessageBox.Show("用户名或密码不能为空！", "提示", MessageBoxButtons.OK, MessageBoxIcon.Information);
                }
                else
                {
                    if (txtName.Text == DataClass.MyMeans.Login_Name)
                    {
                        dataoperate.getsqlcom("update tb_Login set pass ='" + txtPwd.Text + "' where name='" + txtName.Text + "'");
                        lviewBind();
                        txtName.Text = txtPwd.Text = string.Empty;
                    }
                    else
                    {
                        MessageBox.Show("您不是超级用户，没有权限执行此操作！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    }
                }
            }
        }

        private void btnDel_Click(object sender, EventArgs e)
        {
            if (DataClass.MyMeans.Login_Name == "裴成龙")
            {
                if (txtName.Text.ToLower() == "裴成龙")
                {
                    MessageBox.Show("该用户是超级用户，不能删除！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                else
                {
                    dataoperate.getsqlcom("delete from tb_Login where name='" + txtName.Text + "'");
                    lviewBind();
                    txtName.Text = lview.Items[0].Text;
                }
            }
            else
            {
                MessageBox.Show("您不是超级用户，没有权限执行此操作！", "警告", MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }
        }
        private void lview_Click(object sender, EventArgs e)
        {
            txtName.Text = lview.SelectedItems[0].Text;
            txtPwd.Text = string.Empty;
        }
        public void lviewBind()
        {
            lview.Items.Clear();
            ds = dataoperate.getDataSet("select name from tb_Login", "tb_Login");
            foreach (DataRow dr in ds.Tables[0].Rows)
            {
                ListViewItem lvItem = new ListViewItem(dr[0].ToString(), 0);
                lvItem.SubItems.Add(dr[0].ToString());
                lview.Items.Add(lvItem);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
               
                   
                    this.pictureBox2.Load(openpathname);
                
                
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            SaveFileDialog save = new SaveFileDialog();
            save.ShowDialog();
            if (save.FileName != string.Empty)
            {
                pictureBox2.Image.Save(save.FileName);
            }
        }
    }
}
