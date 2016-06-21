using System;
using System.Xml;
using System.Xml.XPath;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace LevelEditorApp
   {
   public partial class XMLControl : UserControl
      {
      public XMLControl()
         {
         InitializeComponent();
         SetViewMode( VIEW_MODE.TABLE );
         }
      private bool m_bGridViewModeReadError = false;
      private DataSet m_DataSet;
      //private List<System.Windows.Forms.DataGridView> m_XMLGridViews;
      public enum VIEW_MODE { XML, TABLE }
      public VIEW_MODE ViewMode
         {
         get
            {
            return ( !this.xmlPanel.Visible ? VIEW_MODE.XML : VIEW_MODE.TABLE );
            }
         set
            {
            SetViewMode( value );
            }
         }

      private string m_sDataFilePath = string.Empty;
      public string DataFilePath
         {
         get
            {
            return m_sDataFilePath;
            }
         set
            {
            m_sDataFilePath = value;
            LoadDataFile();
            }
         }

      private int m_nDataSetTableIndex = 0;
      public int DataSetTableIndex
         {
         get
            {
            return m_nDataSetTableIndex;
            }
         set
            {
            SetDataSetTableIndex( value );
            }
         }

      private int m_nDataTableCount = 0;
      public int DataTableCount
         {
         get { return m_nDataTableCount; }
         }

      private void SetViewMode( VIEW_MODE mode )
         {
         if( m_bGridViewModeReadError == true )
            {
            mode = VIEW_MODE.XML;
            }

         if( mode == VIEW_MODE.XML )
            {
            xmlPanel.Visible = false;
            }
         else
            {
            xmlPanel.Visible = true;
            }
         }

      private void LoadDataFile()
         {
         m_bGridViewModeReadError = false;

         if( ( m_sDataFilePath != string.Empty ) && ( File.Exists( m_sDataFilePath ) == true ) )
            {
            // Creates a DataSet and loads it with the xml content
            try
               {
               XmlReader reader = XmlReader.Create( m_sDataFilePath );
               m_DataSet = new DataSet();
               m_DataSet.ReadXml( reader );
               ///m_DataSet.ReadXml( m_sDataFilePath, XmlReadMode.Auto );
               m_nDataTableCount = m_DataSet.Tables.Count;
               // write to XML file
               //System.IO.StreamWriter xmlSW = new System.IO.StreamWriter( "Customers.xml" );
               //m_DataSet.WriteXml( xmlSW, XmlWriteMode.IgnoreSchema );
               //xmlSW.Close();
               GenerateXMLTreeView();
               GenerateXMLGrieView();
               //    grdTableView.DataSource = dsXmlFile.Tables[DataSetTableIndex];                    
               }
            catch( Exception e )
               {
               m_bGridViewModeReadError = true;
               m_nDataTableCount = 0;
               SetViewMode( VIEW_MODE.XML );
               MessageBox.Show( "Error: " + e.ToString() );
               }
            }
         else
            {
            //    grdTableView.DataSource = null;
            }
         }

      private void SetDataSetTableIndex( int nTableIndex )
         {
         if( nTableIndex >= m_nDataTableCount )
            {
            return;
            }
         m_nDataSetTableIndex = nTableIndex;
         LoadDataFile();
         }

      private void addTreeNode( XmlNode xmlNode, TreeNode treeNode )
         {
         XmlNode xNode;
         TreeNode tNode;
         XmlNodeList xNodeList;
         if( xmlNode.HasChildNodes ) //The current node has children
            {
            xNodeList = xmlNode.ChildNodes;
            for( int x = 0; x <= xNodeList.Count - 1; x++ )
            //Loop through the child nodes
               {
               xNode = xmlNode.ChildNodes[ x ];
               treeNode.Nodes.Add( new TreeNode( xNode.Name ) );
               tNode = treeNode.Nodes[ x ];
               addTreeNode( xNode, tNode );
               }
            }
         else //No children, so add the outer xml (trimming off whitespace)
            treeNode.Text = xmlNode.OuterXml.Trim();
         }

      private void treeViewAfterExpand( object sender, TreeViewEventArgs e )
         {
         
         //int maxRight = this.xmlTreeView.ClientSize.Width;
         //int originWidth = maxRight;
         //if( e.Node.Nodes != null )
         //   foreach( TreeNode node in e.Node.Nodes )
         //      {
         //      maxRight = Math.Max( maxRight, node.Bounds.Right + node.Level * 8 );
         //      }
         //this.xmlTreeView.ClientSize = new Size( maxRight, this.xmlTreeView.ClientSize.Height );
         }

      private void GenerateXMLTreeView()
         {
         try
            {
            //Just a good practice -- change the cursor to a 
            //wait cursor while the nodes populate
            //First, we'll load the Xml document
            XmlDocument xDoc = new XmlDocument();
            xDoc.Load( m_sDataFilePath );
            //Now, clear out the treeview, 
            //and add the first (root) node
            this.xmlTreeView.Nodes.Clear();
            this.xmlTreeView.Nodes.Add( new TreeNode( xDoc.DocumentElement.Name ) );
            TreeNode tNode = new TreeNode();
            tNode = (TreeNode) this.xmlTreeView.Nodes[ 0 ];
            //We make a call to addTreeNode, 
            //where we'll add all of our nodes
            addTreeNode( xDoc.DocumentElement, tNode );
            //Expand the treeview to show all nodes
           // this.xmlTreeView.ExpandAll();

            }
         catch( XmlException xExc )
            //Exception is thrown is there is an error in the Xml
            {
            MessageBox.Show( xExc.Message );
            }
         catch( Exception ex ) //General exception
            {
            MessageBox.Show( ex.Message );
            }
         }

      private void GenerateXMLGrieView()
         {
         for( int i = 0; i < m_DataSet.Tables.Count; ++i )
            {
            // Each table represents an unique XML node name
            // Rows count represents how many node has the same name
            //
            bool hasAttribute = false;
            for( int j = 0; j < m_DataSet.Tables[ i ].Rows[ 0 ].Table.Columns.Count; ++j )
               {
               if( m_DataSet.Tables[ i ].Rows[ 0 ].Table.Columns[ j ].DataType.Name.Equals( "String" ) )
                  {
                  hasAttribute = true;
                  break;
                  }
               }
            if( hasAttribute )
               {
               System.Windows.Forms.DataGridView dataGridView = new System.Windows.Forms.DataGridView();
               System.Windows.Forms.Button button = new Button();
               System.Windows.Forms.ToolTip toolTip1 = new System.Windows.Forms.ToolTip();
               toolTip1.SetToolTip( button, "motherfucker" );
               this.xmlPanel.RowCount = this.xmlPanel.RowCount + 1;
               this.xmlPanel.Controls.Add( dataGridView, 1, this.xmlPanel.RowCount - 1 );
               this.xmlPanel.Controls.Add( button, 0, this.xmlPanel.RowCount - 1 );
               button.Text = ( m_DataSet.Tables[ i ].TableName );

               //if( m_DataSet.Tables[ i ].Rows[ 0 ].Table.ParentRelations.Count > 0 )
               //   {

               //   Console.WriteLine( m_DataSet.Tables[ i ].TableName );
               //   Console.WriteLine( m_DataSet.Tables[ i ].Rows[ 0 ].Table.ParentRelations[ 0 ].RelationName );
               //   Console.WriteLine( m_DataSet.Tables[ i ].Rows[ 0 ].Table.ParentRelations[ 0 ].ParentTable.TableName );
               //   foreach( DataRow aaa in m_DataSet.Tables[ i ].Rows )
               //      {
               //      Console.WriteLine( "********************" );
               //      foreach( DataRelation bbb in aaa.Table.ParentRelations )
               //         {
               //         Console.WriteLine( aaa.TableName );
               //         Console.WriteLine( bbb.RelationName );
               //         }
               //      }
               //   Console.WriteLine( "------------------" );
               //   if( m_DataSet.Tables[ i ].Rows[ 0 ].Table.ParentRelations.Count > 1 )
               //      {
               //      int asdasdasd = 0;
               //      }
               //   }
               //else
               //   {
               //   int asdasdasd = 0;
               //   }

               dataGridView.DataSource = m_DataSet.Tables[ i ];
               dataGridView.AllowUserToAddRows = false;
               dataGridView.AllowUserToDeleteRows = false;
               dataGridView.AllowUserToResizeRows = false;
               dataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
               dataGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;

               dataGridView.BackgroundColor = System.Drawing.Color.FromArgb( ( (int) ( ( (byte) ( 49 ) ) ) ), ( (int) ( ( (byte) ( 49 ) ) ) ), ( (int) ( ( (byte) ( 49 ) ) ) ) );

               var totalHeight = dataGridView.Rows.GetRowsHeight( DataGridViewElementStates.None );
               if( dataGridView.Rows.Count > 0 )
                  {
                  totalHeight += dataGridView.ColumnHeadersHeight;
                  }
               dataGridView.Height = totalHeight;
               this.xmlPanel.RowStyles.Add( new System.Windows.Forms.RowStyle( System.Windows.Forms.SizeType.Absolute, totalHeight ) );
               //     this.xmlPanel.ColumnStyles[ 1 ] = new System.Windows.Forms.ColumnStyle( System.Windows.Forms.SizeType.Absolute, dataGridView.Columns.GetColumnsWidth( DataGridViewElementStates.None ) );
               }
            //if( m_DataSet.Tables[ i ].Rows.Count > 0 )
            //   {
            //   Console.WriteLine(  );
            //   Console.WriteLine("table: " + i + " " + m_DataSet.Tables[ i ].TableName );
            //   for( int j = 0; j < m_DataSet.Tables[ i ].Rows.Count; j++ )
            //      {

            //      int len = m_DataSet.Tables[ i ].Rows[ j ].Table.Rows.Count;
            //      int clen = m_DataSet.Tables[ i ].Rows[ j ].Table.Columns.Count;
            //      int ilen = m_DataSet.Tables[ i ].Rows[ j ].ItemArray.Length;
            //      Console.WriteLine( "row " + j + " " + "coujnt = " + len + " " + clen + " " + ilen );
            //      for( int k = 0; k < m_DataSet.Tables[ i ].Rows[ j ].Table.Columns.Count; ++k )
            //         {
            //         Console.WriteLine( "name " + m_DataSet.Tables[ i ].Rows[ j ].Table.Columns[ k ].ColumnName 
            //            + " " + m_DataSet.Tables[ i ].Rows[ j ].Table.Columns[ k ].DataType.FullName 
            //            + " " + m_DataSet.Tables[ i ].Rows[ j ].Table.Columns[k].Caption
            //            + " " + m_DataSet.Tables[ i ].Rows[ j ].Table.Columns[ k ].DataType.Name );

            //         }

            //      }

            //   }
            //if( true || ( m_DataSet.Tables[ i ].Rows.Count > 0 && m_DataSet.Tables[ i ].Rows[0].ItemArray.Length > 0 ) )
            //   {

            //   }
            }


         }

      private void button1_MouseClick( object sender, MouseEventArgs e )
         {
         if( ViewMode == VIEW_MODE.XML )
            {
            SetViewMode( VIEW_MODE.TABLE );
            }
         else
            {
            SetViewMode( VIEW_MODE.XML );
            }
         }
      }
   }
