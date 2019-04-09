using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Data.SqlClient;



namespace 调用dll并读取图片
{
    public partial class F_SetMonitor : Form
    {
        public F_SetMonitor()
        {
            InitializeComponent();
        }
        DataClass.MyMeans dataoperate = new 调用dll并读取图片.DataClass.MyMeans();
        DataSet ds;

        private void F_SetMonitor_Load(object sender, EventArgs e)
        {
            lviewBind();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            if (DataClass.MyMeans.Login_Name == "mr")
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
            if (DataClass.MyMeans.Login_Name == "mr")
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
            if (DataClass.MyMeans.Login_Name == "mr")
            {
                if (txtName.Text.ToLower() == "mr")
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


    }
}
