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
         }
      private bool m_bGridViewModeReadError = false;
      private DataSet m_DataSet;
      private TextReader m_TextReader;
     
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

      private string m_DataString;
      public string DataString 
         {
         get 
            {
            return m_DataString;
            }
         set 
            {
            m_DataString = value;
            LoadDataString();
            }

         }

      private int m_nDataTableCount = 0;
      public int DataTableCount
         {
         get { return m_nDataTableCount; }
         }

      private void LoadDataSet() 
         {
         DataSet newDataset = new DataSet();
         // Creates a DataSet and loads it with the xml content
         newDataset.ReadXml( m_TextReader );
         bool isSameStructure = ( m_DataSet == null)? false: IsSameDataSetStructure( newDataset );
         m_DataSet = newDataset;
         if( !isSameStructure )
            {
            GenerateXMLTreeView();
            GenerateXMLGridView();
            }
         ///m_DataSet.ReadXml( m_sDataFilePath, XmlReadMode.Auto );
         m_nDataTableCount = m_DataSet.Tables.Count;
         }

      private void ReGenerateView() 
         {
         GenerateXMLTreeView();
         GenerateXMLGridView();
         }

      private void LoadDataFile()
         {
         m_bGridViewModeReadError = false;

         if( ( m_sDataFilePath != string.Empty ) && ( File.Exists( m_sDataFilePath ) == true ) )
            {
            try
               {
               DataString = File.ReadAllText( m_sDataFilePath );
               }
            catch( Exception e )
               {
               m_bGridViewModeReadError = true;
               m_nDataTableCount = 0;
               MessageBox.Show( "Error: " + e.ToString() );
               }
            }
         else
            {
            //    grdTableView.DataSource = null;
            }
         }

      private void LoadDataString() 
         {
         m_bGridViewModeReadError = false;

         if( ( m_DataString != null && m_DataString != string.Empty ) )
            {
            try
               {
               m_TextReader = new StringReader( m_DataString );
               LoadDataSet();
               ReGenerateView();
               }
            catch( Exception e )
               {
               m_bGridViewModeReadError = true;
               m_nDataTableCount = 0;
               MessageBox.Show( "Error: " + e.ToString() );
               }
            }
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

      private int GetExpandedNodeCount( TreeNode tn ) 
         {
         int retCount = 0;
         if( tn.IsExpanded )
            {
            retCount += tn.Nodes.Count;
            foreach( TreeNode child in tn.Nodes)
               {
               if( child.IsExpanded )
                  {
                  retCount += GetExpandedNodeCount( child );
                  }
               }
            }
         
         return retCount;
         }

      private void treeViewAfterExpand( object sender, TreeViewEventArgs e )
         {

         int maxRight = this.xmlTreeView.ClientSize.Width;
         if( e.Node.Nodes != null )
            foreach( TreeNode node in e.Node.Nodes )
               {
               maxRight = Math.Max( maxRight, node.Bounds.Right + node.Level * 8 );
               }

         int nodeHeight = this.xmlTreeView.Nodes[ 0 ].Bounds.Height + 2;

         int openNodeCount = GetExpandedNodeCount( this.xmlTreeView.Nodes[ 0 ] ) + 1;

         int requiredFormHeight = ( openNodeCount * nodeHeight );

         this.xmlTreeView.Size = new Size( maxRight, requiredFormHeight );
         }

      private bool GetParentTableRow( ref String parentNodeName, ref int parentNodeIndex, String curNodeName, int curNodeIndex ) 
         {
         for( int i = 0; i < m_DataSet.Tables[ curNodeName ].Rows[ curNodeIndex ].ItemArray.Length; ++i )
            {
            object element = m_DataSet.Tables[ curNodeName ].Rows[ curNodeIndex ].ItemArray[ i ];
            string typename = element.GetType().Name;
            string elementName = m_DataSet.Tables[ curNodeName ].Rows[ curNodeIndex ].Table.Columns[ i ].ColumnName;
            if( element.GetType().Name.Equals( "Int32" ) && !m_DataSet.Tables[ curNodeName ].Rows[ curNodeIndex ].Table.Columns[ i ].ColumnName.Equals( curNodeName + "_Id" ) )
               {
               string removeString = "_Id";
               int index = elementName.IndexOf( removeString );
               parentNodeName = ( index < 0 ) ? elementName : elementName.Remove( index, removeString.Length );
               parentNodeIndex = Convert.ToInt32( element );
               //Int32 ddd( element );
              
               return true;
               }
            }
         return false;
         }


      private void dataGridView_CellToolTipTextNeeded( object sender, DataGridViewCellToolTipTextNeededEventArgs e)
         {
         //DataGridView dgrv( );
         var dataGridView = sender as System.Windows.Forms.DataGridView;
         var dataTable = dataGridView.DataSource as DataTable;
         if( e.RowIndex >= 0 )
            {
            string currentNodeName = dataTable.TableName; 
            List<String> pathList = new List<string>();
            pathList.Add( currentNodeName );
            int currentRow = e.RowIndex;
            
            while( GetParentTableRow( ref currentNodeName, ref currentRow, currentNodeName, currentRow ) )
               {
               pathList.Add( currentNodeName );
               }
            pathList.Reverse();
            string pathName = "";
            string space = " ";
            foreach( string pathNode in pathList )
               {
               if( pathName.Equals( "" ) )
                  {
                  pathName = pathNode;
                  }
               else
                  {
                  pathName = pathName + "\n" + space + "->" + pathNode;
                  space += " ";
                  }
               
               }
            e.ToolTipText = string.Format( pathName );
            }
         else 
            {
            e.ToolTipText = string.Format( "tip for row {0}, col {1}",
            e.RowIndex, e.ColumnIndex + dataTable.TableName );
            }
         }

      private void GenerateXMLTreeView()
         {
         try
            {
            //Just a good practice -- change the cursor to a 
            //wait cursor while the nodes populate
            //First, we'll load the Xml document
            XmlDocument xDoc = new XmlDocument();
            xDoc.LoadXml( m_DataString );
            //Now, clear out the treeview, 
            //and add the first (root) node
            this.xmlTreeView.Nodes.Clear();
            this.xmlTreeView.Nodes.Add( new TreeNode( xDoc.DocumentElement.Name) );
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

      void dataGridView_CellEndEdit( object sender, DataGridViewCellEventArgs e )
         {
         var dataGridView = sender as System.Windows.Forms.DataGridView;
         // write to XML file
         //System.IO.StreamWriter xmlSW = new System.IO.StreamWriter( "Customers.xml" );
         //m_DataSet.WriteXml( xmlSW, XmlWriteMode.IgnoreSchema );
         //xmlSW.Close();

         StringWriter sw = new StringWriter();
         m_DataSet.WriteXml( sw, XmlWriteMode.IgnoreSchema );
         m_DataString = sw.ToString();
         Program.s_Editor.m_SDLThreadWorker.ModifyActor( m_DataString );
         }

      private void GenerateXMLGridView()
         {
         this.xmlPanel.AutoScroll = true;
         this.xmlPanel.AutoSize = true;
         this.xmlPanel.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
         this.xmlPanel.Size = new System.Drawing.Size( 0, 0 );
         this.xmlPanel.Controls.Clear();
         this.xmlPanel.RowCount = 0;
         this.xmlPanel.RowStyles.Clear();
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
               System.Windows.Forms.Label label = new System.Windows.Forms.Label();
               label.BackColor = System.Drawing.Color.FromArgb( ( (int) ( ( (byte) ( 120 ) ) ) ), ( (int) ( ( (byte) ( 120 ) ) ) ), ( (int) ( ( (byte) ( 120 ) ) ) ) );
               label.AutoSize = true;
               label.Anchor = ( (System.Windows.Forms.AnchorStyles) ( ( System.Windows.Forms.AnchorStyles.Top | 
                                                                        System.Windows.Forms.AnchorStyles.Left | 
                                                                        System.Windows.Forms.AnchorStyles.Right ) ) );
               this.xmlPanel.RowCount = this.xmlPanel.RowCount + 1;
               this.xmlPanel.Controls.Add( dataGridView, 1, this.xmlPanel.RowCount - 1 );
               this.xmlPanel.Controls.Add( label, 0, this.xmlPanel.RowCount - 1 );

               label.Text = ( m_DataSet.Tables[ i ].TableName );
               
               

               //for( int j = 0; j < m_DataSet.Tables[ i ].Rows[0].Table.ParentRelations.Count; ++j )
               //   {
               //   m_DataSet.Tables[i].Rows[0].GetP
               //   DataTable table = m_DataSet.Tables[ i ].Rows[ 0 ].Table;
               //   String relationStr = table.TableName;
               //   table = table.ParentRelations[ j ].ParentTable;
               //   relationStr = table.TableName + "->" + relationStr;
               //   while( table.ParentRelations.Count > 0 )
               //      {

               //      table = table.ParentRelations[ 0 ].ParentTable;
               //      relationStr = table.TableName + "->" + relationStr;
               //      }
               //   Console.WriteLine( relationStr );
               //   }
            //   

               //if( .ParentRelations.Count > 0 )
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
               dataGridView.AllowUserToResizeColumns = false;
               dataGridView.AllowUserToOrderColumns = false;
               dataGridView.ColumnHeadersHeightSizeMode = DataGridViewColumnHeadersHeightSizeMode.DisableResizing;
               dataGridView.RowHeadersWidthSizeMode = DataGridViewRowHeadersWidthSizeMode.DisableResizing;
               dataGridView.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
               dataGridView.AutoSizeRowsMode = System.Windows.Forms.DataGridViewAutoSizeRowsMode.AllCells;

               dataGridView.BackgroundColor = System.Drawing.Color.FromArgb( ( (int) ( ( (byte) ( 49 ) ) ) ), ( (int) ( ( (byte) ( 49 ) ) ) ), ( (int) ( ( (byte) ( 49 ) ) ) ) );


               int totalHeight = 24 * dataGridView.RowCount + 25;

               //var totalHeight = dataGridView.Rows.GetRowsHeight( DataGridViewElementStates.None );
               //totalHeight += dataGridView.ColumnHeadersHeight;
               
               //dataGridView.Height = totalHeight;
               this.xmlPanel.RowStyles.Add( new System.Windows.Forms.RowStyle( System.Windows.Forms.SizeType.Absolute, totalHeight ) );
               dataGridView.CellToolTipTextNeeded += new DataGridViewCellToolTipTextNeededEventHandler( this.dataGridView_CellToolTipTextNeeded );
               dataGridView.CellEndEdit += new DataGridViewCellEventHandler( this.dataGridView_CellEndEdit );

               //     this.xmlPanel.ColumnStyles[ 1 ] = new System.Windows.Forms.ColumnStyle( System.Windows.Forms.SizeType.Absolute, dataGridView.Columns.GetColumnsWidth( DataGridViewElementStates.None ) );
               //Console.WriteLine( "----TABLE : " + m_DataSet.Tables[ i ].TableName );
               //for( int j = 0; j < m_DataSet.Tables[ i ].Rows.Count; ++j )
               //   {
               //   Console.WriteLine( "row" + j );
               //   string parentName = "";
               //   int parentRow = -1;
                  
               //   if( GetParentTableRow( ref parentName, ref parentRow, m_DataSet.Tables[ i ].TableName, j ) )
               //      {
               //      Console.WriteLine( "None Root Node! " + parentName + parentRow );
               //      }
               //   else
               //      {
               //      Console.WriteLine( "Root Node! " );
               //      }
               //   //if( m_DataSet.Tables[ i ].Rows[ j ].Table.ParentRelations.Count == 0 )
               //   //   {
               //   //   Console.WriteLine( " no parent" );
               //   //   }
               //   //else
               //   //   {
               //   //   for( int k = 0; k < m_DataSet.Tables[ i ].Rows[ j ].Table.ParentRelations.Count; ++k )
               //   //   {
               //   //   Console.WriteLine( m_DataSet.Tables[ i ].Rows[ j ].Table.ParentRelations[k].RelationName );
               //   //   }
               //   //   }
               //   //for( int k = 0; k < m_DataSet.Tables[ i ].Rows[ j ].ItemArray.Length; ++k )
               //   //   {
               //   //   if( m_DataSet.Tables[ i ].Rows[ j ].ItemArray[ k ] == null )
               //   //      {
               //   //      Console.WriteLine( "empty!" );
               //   //      }
               //   //   Console.WriteLine( m_DataSet.Tables[ i ].Rows[ j ].Table.Columns[ k ].ColumnName + " " + "\"" + m_DataSet.Tables[ i ].Rows[ j ].ItemArray[ k ] + "\"" );
               //   //   }
               //   }
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

      private bool IsSameDataSetStructure( DataSet other )
         {
         if( m_DataSet.Tables.Count != other.Tables.Count )
            {
            return false;
            }
         for( int table = 0; table < m_DataSet.Tables.Count; ++table )
            {
            if( !m_DataSet.Tables[table].TableName.Equals( other.Tables[table].TableName ) )
               {
               return false;
               }
            if( m_DataSet.Tables[table].Rows.Count != other.Tables[table].Rows.Count  )
               {
               return false;
               }
            for( int row = 0; row < m_DataSet.Tables[ table ].Rows.Count; ++row )
               {
               if( m_DataSet.Tables[table].Rows[ row ].Table.Columns.Count != other.Tables[table].Rows[ row ].Table.Columns.Count )
                  {
                  return false;
                  }
               for( int col = 0; col < m_DataSet.Tables[ table ].Rows[ row ].Table.Columns.Count; ++col )
                  {
                  if( !m_DataSet.Tables[ table ].Rows[ row ].Table.Columns[ col ].DataType.Name.Equals( other.Tables[ table ].Rows[ row ].Table.Columns[ col ].DataType.Name ) )
                     {
                     return false;
                     }
                  if( !m_DataSet.Tables[ table ].Rows[ row ].Table.Columns[ col ].ColumnName.Equals( other.Tables[ table ].Rows[ row ].Table.Columns[ col ].ColumnName ) )
                     {
                     return false;
                     }
                  }
               }
            }
         return true;
         }
      }
   }
