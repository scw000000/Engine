using System;
using System.Collections.Generic;
using System.Collections;
using System.ComponentModel;
using System.Data;
using System.Drawing.Drawing2D;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace LevelEditorApp
   {
   class TabControlEX : System.Windows.Forms.TabControl
      {
      private const int m_CloseRectHorDistance = 2;
      private const int m_CloseRectVerDistance = 2;
      private const float m_InnerCrossDistance = 1.5f;
      private float m_MaxStringLength = -1.0f;
      private const int m_MinTabSize = 50;
      private int m_MouseOnCloseBtnPageIndx = -1;
      /// <summary>
      /// Required designer variable.
      /// </summary>
      private System.ComponentModel.Container components = null;
      public delegate void OnHeaderCloseDelegate( object sender, CloseEventArgs e );
      public event OnHeaderCloseDelegate OnClose;
      public TabControlEX()
         {
         // This call is required by the Windows.Forms Form Designer.
         InitializeComponent();
         this.TabStop = false;
         this.OnClose += new OnHeaderCloseDelegate( this.OnTabPageClose );

         // TODO: Add any initialization after the InitComponent call

         }
      private bool confirmOnClose = true;
      public bool ConfirmOnClose
         {
         get
            {
            return this.confirmOnClose;
            }
         set
            {
            this.confirmOnClose = value;
            }
         }

      /// <summary>
      /// Clean up any resources being used.
      /// </summary>
      protected override void Dispose( bool disposing )
         {
         if( disposing )
            {
            if( components != null )
               components.Dispose();
            }
         base.Dispose( disposing );
         }


      #region Component Designer generated code
      /// <summary>
      /// Required method for Designer support - do not modify 
      /// the contents of this method with the code editor.
      /// </summary>
      private void InitializeComponent()
         {
         components = new System.ComponentModel.Container();
         SetStyle( System.Windows.Forms.ControlStyles.DoubleBuffer, true );
         this.TabStop = false;
         //       this.DrawMode = System.Windows.Forms.TabDrawMode.OwnerDrawFixed;
         //       this.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
         //      this.ItemSize = new System.Drawing.Size(30, 24);
         //this.Controls.Add(this.btnClose); 
         }
      #endregion
      private Stream GetContentFromResource( string filename )
         {
         Assembly asm = Assembly.GetExecutingAssembly();
         Stream stream = asm.GetManifestResourceStream( "MyControlLibrary." + filename );
         return stream;

         }

      protected void DrawCloseBox( Graphics g, RectangleF tabTextArea, LinearGradientBrush _Brush )
         {
         float outerCloseBtnWidth = tabTextArea.Height - 3;
         float outerCloerBtnHeight = tabTextArea.Height - 5;
         float outerCloseBtnX = tabTextArea.X + tabTextArea.Width - outerCloseBtnWidth - 2;
         float outerCloseBtnY = tabTextArea.Y + ( tabTextArea.Height - outerCloerBtnHeight ) / 2;

         g.FillRectangle( _Brush, outerCloseBtnX, outerCloseBtnY, outerCloseBtnWidth, outerCloerBtnHeight );

         float innerCloseBtnWidth = outerCloseBtnWidth - m_CloseRectHorDistance * 2;
         float innerCloerBtnHeight = outerCloerBtnHeight - m_CloseRectVerDistance * 2;
         float innerCloseBtnX = outerCloseBtnX + m_CloseRectHorDistance;
         float innerCloseBtnY = outerCloseBtnY + m_CloseRectVerDistance;

         g.DrawRectangle( Pens.White,
         innerCloseBtnX,
         innerCloseBtnY,
         innerCloseBtnWidth,
         innerCloerBtnHeight );
         using( Pen pen = new Pen( Color.White, 2 ) )
            {
            g.DrawLine( pen, innerCloseBtnX + m_InnerCrossDistance, innerCloseBtnY + m_InnerCrossDistance, innerCloseBtnX + innerCloseBtnWidth - m_InnerCrossDistance, innerCloseBtnY + innerCloerBtnHeight - m_InnerCrossDistance );
            g.DrawLine( pen, innerCloseBtnX + m_InnerCrossDistance, innerCloseBtnY + innerCloerBtnHeight - m_InnerCrossDistance, innerCloseBtnX + innerCloseBtnWidth - m_InnerCrossDistance, innerCloseBtnY + m_InnerCrossDistance );
            }
         }

      protected float MeasureTabWidth( int index ) 
         {
         TabPage curTab = this.TabPages[ index ];
         Graphics g = Graphics.FromHwnd( curTab.Handle );
         SizeF size = g.MeasureString( curTab.Text, curTab.Font, 300 );
         return size.Width + this.GetTabRect( 0 ).Height + 11;
         }

      protected override void OnControlAdded( ControlEventArgs e )
         {
         base.OnControlAdded( e );
         this.SelectedIndex = this.TabCount - 1;
         TabPage curTab = this.TabPages[ this.TabCount - 1 ];
         Graphics g = Graphics.FromHwnd( curTab.Handle );
         SizeF size = g.MeasureString( curTab.Text, curTab.Font, 300 );
         float newTabMinWidth = size.Width + this.GetTabRect( 0 ).Height + 11;
         if( m_MaxStringLength < newTabMinWidth )
            {
            m_MaxStringLength = newTabMinWidth;
            this.ItemSize = new Size( Math.Max( m_MinTabSize, (int) m_MaxStringLength ), this.ItemSize.Height );

            }

         }

      protected override void OnControlRemoved( ControlEventArgs e )
         {
         base.OnControlRemoved( e );
         m_MaxStringLength = -1;
         
         for( int i = 0; i < this.TabCount; ++i )
            {
            if( !e.Control.Name.Equals( this.TabPages[ i ].Name ) && m_MaxStringLength < MeasureTabWidth( i ) )
               {
               m_MaxStringLength = MeasureTabWidth( i );
               }
            }
         if( m_MaxStringLength > 0 )
            {
            this.ItemSize = new Size( Math.Max( m_MinTabSize, (int) m_MaxStringLength ), this.ItemSize.Height );
            }

         }


      protected override void OnDrawItem( DrawItemEventArgs e )
         {
         //draw rectangle behind the tabs
         Rectangle lasttabrect = this.GetTabRect( TabPages.Count - 1 );
         Rectangle background = new Rectangle();
         background.Location = new Point( lasttabrect.Right, 0 );

         //pad the rectangle to cover the 1 pixel line between the top of the tabpage and the start of the tabs
         background.Size = new Size( Right - background.Left, lasttabrect.Height + 1 );
         SolidBrush fillbrush = new SolidBrush( Color.FromArgb( 255, 49, 49, 49 ) );
         e.Graphics.FillRectangle( fillbrush, background );



         if( e.Bounds != RectangleF.Empty )
            {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;

            RectangleF tabTextArea = RectangleF.Empty;
            if( e.Index != this.SelectedIndex )
               {
               tabTextArea = (RectangleF) this.GetTabRect( e.Index );
               GraphicsPath _Path = new GraphicsPath();
               _Path.AddRectangle( tabTextArea );
               using( LinearGradientBrush _Brush = new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight, LinearGradientMode.Vertical ) )
                  {
                  ColorBlend _ColorBlend = new ColorBlend( 3 );
                  _ColorBlend.Colors = new Color[]{Color.FromArgb(255, 49, 49, 49 ), 
                                                      Color.FromArgb( 255, 49, 49, 49 ), 
                                                      Color.FromArgb( 255, 49, 49, 49 ),
                                                      Color.FromArgb( 255, 49, 49, 49 )};
                  _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                  _Brush.InterpolationColors = _ColorBlend;

                  e.Graphics.FillPath( _Brush, _Path );
                  using( Pen pen = new Pen( SystemColors.ActiveBorder ) )
                     {
                     e.Graphics.DrawPath( pen, _Path );
                     }

                  if( CanDrawCloseButton( e.Index ) )
                     {
                     if( m_MouseOnCloseBtnPageIndx == e.Index )
                        {
                        _ColorBlend.Colors = new Color[]
                           {
                              Color.FromArgb(255, 252, 193, 183),
                              Color.FromArgb(255, 252, 193, 183), 
                              Color.FromArgb(255, 210, 35, 2),
                              Color.FromArgb(255, 210, 35, 2)
                           };
                        Console.Write( "detected!@!" );
                        }
                     else
                        {
                        _ColorBlend.Colors = new Color[]{  SystemColors.ActiveBorder, 
                                                        SystemColors.ActiveBorder,SystemColors.ActiveBorder,
                                                        SystemColors.ActiveBorder};
                        }

                     _Brush.InterpolationColors = _ColorBlend;
                     DrawCloseBox( e.Graphics, tabTextArea, _Brush );
                     }
                  if( CanDrawMenuButton( e.Index ) )
                     {
                     _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                     // assign the color blend to the pathgradientbrush
                     _Brush.InterpolationColors = _ColorBlend;

                     e.Graphics.FillRectangle( _Brush, tabTextArea.X + tabTextArea.Width - 43, 4, tabTextArea.Height - 3, tabTextArea.Height - 5 );
                     // e.Graphics.DrawRectangle(SystemPens.GradientInactiveCaption, tabTextArea.X + tabTextArea.Width - 37, 7, 13, 13);
                     e.Graphics.DrawRectangle( new Pen( Color.White ), tabTextArea.X + tabTextArea.Width - 41, 6, tabTextArea.Height - 7, tabTextArea.Height - 9 );
                     using( Pen pen = new Pen( Color.White, 2 ) )
                        {
                        e.Graphics.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 36, 11, tabTextArea.X + tabTextArea.Width - 33, 16 );
                        e.Graphics.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 33, 16, tabTextArea.X + tabTextArea.Width - 30, 11 );
                        }
                     }
                  }
               _Path.Dispose();
               }
            else
               {

               tabTextArea = (RectangleF) this.GetTabRect( e.Index );
               GraphicsPath _Path = new GraphicsPath();
               _Path.AddRectangle( tabTextArea );

               using( LinearGradientBrush _Brush = new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight, LinearGradientMode.Vertical ) )
                  {
                  ColorBlend _ColorBlend = new ColorBlend( 3 );
                  _ColorBlend.Colors = new Color[]{Color.FromArgb(255, 120,120,120), 
                                                         Color.FromArgb(255, 100,100,100),
                                                         Color.FromArgb(255, 80,80,80),
                                                         Color.FromArgb(255,64,64,64)};
                  _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                  _Brush.InterpolationColors = _ColorBlend;
                  e.Graphics.FillPath( _Brush, _Path );
                  using( Pen pen = new Pen( SystemColors.ActiveBorder ) )
                     {
                     e.Graphics.DrawPath( pen, _Path );
                     }
                  if( CanDrawCloseButton( e.Index ) )
                     {
                     Console.Write( m_MouseOnCloseBtnPageIndx );
                     if( m_MouseOnCloseBtnPageIndx == e.Index )
                        {
                        _ColorBlend.Colors = new Color[]
                           {
                              Color.FromArgb(255, 252, 193, 183),
                              Color.FromArgb(255, 252, 193, 183), 
                              Color.FromArgb(255, 210, 35, 2),
                              Color.FromArgb(255, 210, 35, 2)
                           };
                        Console.Write( "detected!@!" );
                        }
                     else
                        {
                        //Drawing Close Button
                        _ColorBlend.Colors = new Color[]{Color.FromArgb(255,231,164,152), 
                                                         Color.FromArgb(255,231,164,152),
                                                         Color.FromArgb(255,197,98,79),
                                                         Color.FromArgb(255,197,98,79)};
                        }

                     _Brush.InterpolationColors = _ColorBlend;
                     DrawCloseBox( e.Graphics, tabTextArea, _Brush );
                     }
                  if( CanDrawMenuButton( e.Index ) )
                     {
                     //Drawing menu button
                     _ColorBlend.Colors = new Color[]{ Color.FromArgb(255,170,213,243), 
                                                            Color.FromArgb(255,170,213,243),
                                                            Color.FromArgb(255,44,137,191),
                                                            Color.FromArgb(255,44,137,191)};
                     _Brush.InterpolationColors = _ColorBlend;
                     e.Graphics.FillRectangle( _Brush, tabTextArea.X + tabTextArea.Width - 43, 4, tabTextArea.Height - 3, tabTextArea.Height - 5 );
                     e.Graphics.DrawRectangle( Pens.White, tabTextArea.X + tabTextArea.Width - 41, 6, tabTextArea.Height - 7, tabTextArea.Height - 9 );
                     using( Pen pen = new Pen( Color.White, 2 ) )
                        {
                        e.Graphics.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 36, 11, tabTextArea.X + tabTextArea.Width - 33, 16 );
                        e.Graphics.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 33, 16, tabTextArea.X + tabTextArea.Width - 30, 11 );
                        }
                     }
                  }
               _Path.Dispose();
               }
            StringFormat stringFormat = new StringFormat();
            stringFormat.Alignment = StringAlignment.Near;
            e.Graphics.DrawString( this.TabPages[ e.Index ].Text, e.Font, Brushes.WhiteSmoke, e.Bounds.Left + 5, e.Bounds.Top + 6, stringFormat );
            }

         }

      private bool CanDrawCloseButton( int nIndex )
         {
         if( ( (TabPageEX) this.TabPages[ nIndex ] ).CloseBtnEnabled )
            {
            return true;
            }

         return false;
         }
      private bool CanDrawMenuButton( int nIndex )
         {
         if( ( (TabPageEX) this.TabPages[ nIndex ] ).Menu != null )
            return true;
         return false;
         }
      protected override void OnMouseLeave( EventArgs e )
         {
         m_MouseOnCloseBtnPageIndx = -1;
         Graphics g = CreateGraphics();
         g.SmoothingMode = SmoothingMode.AntiAlias;
         RectangleF tabTextArea = RectangleF.Empty;
         for( int nIndex = 0; nIndex < this.TabCount; nIndex++ )
            {
            if( nIndex != this.SelectedIndex )
               {
               tabTextArea = (RectangleF) this.GetTabRect( nIndex );
               GraphicsPath _Path = new GraphicsPath();
               _Path.AddRectangle( tabTextArea );
               using( LinearGradientBrush _Brush = new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight, LinearGradientMode.Vertical ) )
                  {
                  ColorBlend _ColorBlend = new ColorBlend( 3 );
                  if( CanDrawCloseButton( nIndex ) )
                     {
                     _ColorBlend.Colors = new Color[]{  SystemColors.ActiveBorder, 
                                                     SystemColors.ActiveBorder,SystemColors.ActiveBorder,
                                                     SystemColors.ActiveBorder};

                     _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                     _Brush.InterpolationColors = _ColorBlend;
                     DrawCloseBox( g, tabTextArea, _Brush );
                     }

                  //g.FillRectangle(_Brush, tabTextArea.X + tabTextArea.Width - 22, 4, tabTextArea.Height - 2, tabTextArea.Height - 5);
                  //g.DrawRectangle(Pens.White, tabTextArea.X + tabTextArea.Width - 20, 6, tabTextArea.Height - 8, tabTextArea.Height - 9);
                  //using (Pen pen = new Pen(Color.White, 2))
                  //{
                  //    g.DrawLine(pen, tabTextArea.X + tabTextArea.Width - 16, 9, tabTextArea.X + tabTextArea.Width - 7, 17);
                  //    g.DrawLine(pen, tabTextArea.X + tabTextArea.Width - 16, 17, tabTextArea.X + tabTextArea.Width - 7, 9);
                  //}
                  if( CanDrawMenuButton( nIndex ) )
                     {
                     _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                     // assign the color blend to the pathgradientbrush
                     _Brush.InterpolationColors = _ColorBlend;

                     g.FillRectangle( _Brush, tabTextArea.X + tabTextArea.Width - 43, 4, tabTextArea.Height - 3, tabTextArea.Height - 5 );
                     // e.Graphics.DrawRectangle(SystemPens.GradientInactiveCaption, tabTextArea.X + tabTextArea.Width - 37, 7, 13, 13);
                     g.DrawRectangle( new Pen( Color.White ), tabTextArea.X + tabTextArea.Width - 41, 6, tabTextArea.Height - 7, tabTextArea.Height - 9 );
                     using( Pen pen = new Pen( Color.White, 2 ) )
                        {
                        g.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 36, 11, tabTextArea.X + tabTextArea.Width - 33, 16 );
                        g.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 33, 16, tabTextArea.X + tabTextArea.Width - 30, 11 );
                        }
                     }
                  }
               _Path.Dispose();

               }
            else
               {

               tabTextArea = (RectangleF) this.GetTabRect( nIndex );
               GraphicsPath _Path = new GraphicsPath();
               _Path.AddRectangle( tabTextArea );
               using( LinearGradientBrush _Brush = new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight, LinearGradientMode.Vertical ) )
                  {
                  ColorBlend _ColorBlend = new ColorBlend( 3 );
                  if( CanDrawCloseButton( nIndex ) )
                     {
                     _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };

                     _ColorBlend.Colors = new Color[]{Color.FromArgb(255,231,164,152), 
                                                      Color.FromArgb(255,231,164,152),Color.FromArgb(255,197,98,79),
                                                      Color.FromArgb(255,197,98,79)};
                     _Brush.InterpolationColors = _ColorBlend;
                     DrawCloseBox( g, tabTextArea, _Brush );
                     }

                  //g.FillRectangle(_Brush, tabTextArea.X + tabTextArea.Width - 22, 4, tabTextArea.Height - 3, tabTextArea.Height - 5);
                  //g.DrawRectangle(Pens.White, tabTextArea.X + tabTextArea.Width - 20, 6, tabTextArea.Height - 8, tabTextArea.Height - 9);
                  //using (Pen pen = new Pen(Color.White, 2))
                  //{
                  //    g.DrawLine(pen, tabTextArea.X + tabTextArea.Width - 16, 9, tabTextArea.X + tabTextArea.Width - 7, 17);
                  //    g.DrawLine(pen, tabTextArea.X + tabTextArea.Width - 16, 17, tabTextArea.X + tabTextArea.Width - 7, 9);
                  //}
                  if( CanDrawMenuButton( nIndex ) )
                     {
                     //Drawing menu button
                     _ColorBlend.Colors = new Color[]{SystemColors.ControlLightLight, 
                                                     Color.FromArgb(255,SystemColors.ControlLight),SystemColors.ControlDark,
                                                     SystemColors.ControlLightLight};
                     _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                     _Brush.InterpolationColors = _ColorBlend;
                     _ColorBlend.Colors = new Color[]{Color.FromArgb(255,170,213,243), 
                                                      Color.FromArgb(255,170,213,243),Color.FromArgb(255,44,137,191),
                                                      Color.FromArgb(255,44,137,191)};
                     _Brush.InterpolationColors = _ColorBlend;
                     g.FillRectangle( _Brush, tabTextArea.X + tabTextArea.Width - 43, 4, tabTextArea.Height - 3, tabTextArea.Height - 5 );
                     g.DrawRectangle( Pens.White, tabTextArea.X + tabTextArea.Width - 41, 6, tabTextArea.Height - 7, tabTextArea.Height - 9 );
                     using( Pen pen = new Pen( Color.White, 2 ) )
                        {
                        g.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 36, 11, tabTextArea.X + tabTextArea.Width - 33, 16 );
                        g.DrawLine( pen, tabTextArea.X + tabTextArea.Width - 33, 16, tabTextArea.X + tabTextArea.Width - 30, 11 );
                        }
                     }
                  }
               _Path.Dispose();
               }

            }

         g.Dispose();


         }

      protected override void OnMouseMove( MouseEventArgs e )
         {
         m_MouseOnCloseBtnPageIndx = -1;
         if( !DesignMode )
            {
            Graphics g = CreateGraphics();
            g.SmoothingMode = SmoothingMode.AntiAlias;
            for( int nIndex = 0; nIndex < this.TabCount; nIndex++ )
               {
               RectangleF tabTextArea = (RectangleF) this.GetTabRect( nIndex );
               RectangleF origTextArea = tabTextArea;
               float outerCloseBtnWidth = tabTextArea.Height - 3;
               float outerCloerBtnHeight = tabTextArea.Height - 5;
               float outerCloseBtnX = tabTextArea.X + tabTextArea.Width - outerCloseBtnWidth - 2;
               float outerCloseBtnY = tabTextArea.Y + ( tabTextArea.Height - outerCloerBtnHeight ) / 2;
               Point pt = new Point( e.X, e.Y );
               if( CanDrawCloseButton( nIndex ) )
                  {
                  tabTextArea = new RectangleF( outerCloseBtnX, outerCloseBtnY, outerCloseBtnWidth,
                                     outerCloerBtnHeight );


                  if( tabTextArea.Contains( pt ) )
                     {
                     m_MouseOnCloseBtnPageIndx = nIndex;
                     using(
                         LinearGradientBrush _Brush =
                             new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight,
                                                     LinearGradientMode.Vertical ) )
                        {
                        ColorBlend _ColorBlend = new ColorBlend( 3 );
                        _ColorBlend.Colors = new Color[]
                              {
                                  Color.FromArgb(255, 252, 193, 183),
                                  Color.FromArgb(255, 252, 193, 183), Color.FromArgb(255, 210, 35, 2),
                                  Color.FromArgb(255, 210, 35, 2)
                              };
                        _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                        _Brush.InterpolationColors = _ColorBlend;

                        DrawCloseBox( g, origTextArea, _Brush );
                        //g.FillRectangle( _Brush, tabTextArea );
                        //g.DrawRectangle( Pens.White, tabTextArea.X + 2, 6, tabTextArea.Height - 3,
                        //                tabTextArea.Height - 4 );
                        //using( Pen pen = new Pen( Color.White, 2 ) )
                        //   {
                        //g.DrawLine( pen, tabTextArea.X + 6, 9, tabTextArea.X + 15, 17 );
                        //g.DrawLine( pen, tabTextArea.X + 6, 17, tabTextArea.X + 15, 9 );
                        //   }
                        }
                     }
                  else
                     {
                     if( nIndex != SelectedIndex )
                        {
                        using(
                            LinearGradientBrush _Brush =
                                new LinearGradientBrush( tabTextArea, SystemColors.Control, SystemColors.ControlLight,
                                                        LinearGradientMode.Vertical ) )
                           {
                           ColorBlend _ColorBlend = new ColorBlend( 3 );
                           _ColorBlend.Colors = new Color[]
                                  {
                                      SystemColors.ActiveBorder,
                                      SystemColors.ActiveBorder, SystemColors.ActiveBorder,
                                      SystemColors.ActiveBorder
                                  };
                           _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                           _Brush.InterpolationColors = _ColorBlend;
                           DrawCloseBox( g, origTextArea, _Brush );
                           //g.FillRectangle( _Brush, tabTextArea );
                           //g.DrawRectangle( Pens.White, tabTextArea.X + 2, 6, tabTextArea.Height - 3,
                           //                tabTextArea.Height - 4 );
                           //using( Pen pen = new Pen( Color.White, 2 ) )
                           //   {
                           //g.DrawLine( pen, tabTextArea.X + 6, 9, tabTextArea.X + 15, 17 );
                           //g.DrawLine( pen, tabTextArea.X + 6, 17, tabTextArea.X + 15, 9 );
                           //   }
                           }
                        }
                     }
                  }

               if( CanDrawMenuButton( nIndex ) )
                  {
                  RectangleF tabMenuArea = (RectangleF) this.GetTabRect( nIndex );
                  tabMenuArea =
                       new RectangleF( tabMenuArea.X + tabMenuArea.Width - 43, 4, tabMenuArea.Height - 3,
                                      tabMenuArea.Height - 5 );
                  pt = new Point( e.X, e.Y );
                  if( tabMenuArea.Contains( pt ) )
                     {
                     using(
                         LinearGradientBrush _Brush =
                             new LinearGradientBrush( tabMenuArea, SystemColors.Control, SystemColors.ControlLight,
                                                     LinearGradientMode.Vertical ) )
                        {
                        ColorBlend _ColorBlend = new ColorBlend( 3 );
                        _ColorBlend.Colors = new Color[]
                                  {
                                      Color.FromArgb(255, 170, 213, 255),
                                      Color.FromArgb(255, 170, 213, 255), Color.FromArgb(255, 44, 157, 250),
                                      Color.FromArgb(255, 44, 157, 250)
                                  };
                        _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                        _Brush.InterpolationColors = _ColorBlend;

                        g.FillRectangle( _Brush, tabMenuArea );
                        g.DrawRectangle( Pens.White, tabMenuArea.X + 2, 6, tabMenuArea.Height - 2,
                                        tabMenuArea.Height - 4 );
                        using( Pen pen = new Pen( Color.White, 2 ) )
                           {
                           g.DrawLine( pen, tabMenuArea.X + 7, 11, tabMenuArea.X + 10, 16 );
                           g.DrawLine( pen, tabMenuArea.X + 10, 16, tabMenuArea.X + 13, 11 );
                           }
                        }
                     }
                  else
                     {
                     if( nIndex != SelectedIndex )
                        {
                        using( LinearGradientBrush _Brush = new LinearGradientBrush( tabMenuArea,
                                                                                      SystemColors.Control,
                                                                                      SystemColors.ControlLight,
                                                                                      LinearGradientMode.Vertical ) )
                           {
                           ColorBlend _ColorBlend = new ColorBlend( 3 );
                           _ColorBlend.Colors = new Color[]{ SystemColors.ActiveBorder, 
                                                              SystemColors.ActiveBorder, 
                                                              SystemColors.ActiveBorder,
                                                              SystemColors.ActiveBorder };
                           _ColorBlend.Positions = new float[] { 0f, .4f, 0.5f, 1f };
                           _Brush.InterpolationColors = _ColorBlend;

                           g.FillRectangle( _Brush, tabMenuArea );
                           g.DrawRectangle( Pens.White, tabMenuArea.X + 2, 6, tabMenuArea.Height - 2,
                                                  tabMenuArea.Height - 4 );
                           using( Pen pen = new Pen( Color.White, 2 ) )
                              {
                              g.DrawLine( pen, tabMenuArea.X + 7, 11, tabMenuArea.X + 10, 16 );
                              g.DrawLine( pen, tabMenuArea.X + 10, 16, tabMenuArea.X + 13, 11 );
                              }
                           }
                        }
                     }
                  }

               }
            g.Dispose();
            }
         }

      protected override void OnMouseDown( MouseEventArgs e )
         {
         if( !DesignMode )
            {
            RectangleF tabTextArea = (RectangleF) this.GetTabRect( SelectedIndex );
            float outerCloseBtnWidth = tabTextArea.Height - 3;
            float outerCloerBtnHeight = tabTextArea.Height - 5;
            float outerCloseBtnX = tabTextArea.X + tabTextArea.Width - outerCloseBtnWidth - 2;
            float outerCloseBtnY = tabTextArea.Y + ( tabTextArea.Height - outerCloerBtnHeight ) / 2;

            tabTextArea = new RectangleF( outerCloseBtnX, outerCloseBtnY, outerCloseBtnWidth, outerCloerBtnHeight );
            Point pt = new Point( e.X, e.Y );
            if( CanDrawCloseButton( SelectedIndex ) )
               {
               if( tabTextArea.Contains( pt ) )
                  {
                  if( confirmOnClose )
                     {
                     EditorTextBox textBox = ( ( TabPageEX ) this.TabPages[ SelectedIndex ] ).TextBox;
                     // if textBox exist, ask it instead of the tabPage
                     if( textBox != null )
                        {
                        if( textBox.ConfirmClose() == false )
                           {
                           return;
                           }
                        }
                     else 
                        {
                        if( MessageBox.Show(
                             "You are about to close " + this.TabPages[ SelectedIndex ].Text +
                             " tab. Are you sure you want to continue?", "Confirm close", MessageBoxButtons.YesNo ) ==
                         DialogResult.No ) 
                           {
                           return;
                           }
                        }
                     }
                  //Fire Event to Client
                  if( OnClose != null )
                     {
                     OnClose( this, new CloseEventArgs( SelectedIndex ) );
                     }
                  }
               }

            if( CanDrawMenuButton( SelectedIndex ) )
               {
               RectangleF tabMenuArea = (RectangleF) this.GetTabRect( SelectedIndex );
               tabMenuArea = new RectangleF( tabMenuArea.X + tabMenuArea.Width - 43, 4, tabMenuArea.Height - 3,
                                     tabMenuArea.Height - 5 );
               pt = new Point( e.X, e.Y );
               if( tabMenuArea.Contains( pt ) )
                  {
                  if( ( (TabPageEX) this.TabPages[ SelectedIndex ] ).Menu != null )
                     {
                     ( (TabPageEX) this.TabPages[ SelectedIndex ] ).Menu.Show( this,
                                                                               new Point( (int) tabMenuArea.X,
                                                                                         (int)
                                                                                         ( tabMenuArea.Y +
                                                                                          tabMenuArea.Height ) ) );
                     }
                  }
               }
            }
         }

      private void OnTabPageClose( object sender, CloseEventArgs e )
         {
         this.Controls.Remove( this.TabPages[ e.TabIndex ] );
         }
      }

   public class CloseEventArgs : EventArgs
      {
      private int nTabIndex = -1;
      public CloseEventArgs( int nTabIndex )
         {
         this.nTabIndex = nTabIndex;
         }
      /// <summary>
      /// Get/Set the tab index value where the close button is clicked
      /// </summary>
      public int TabIndex
         {
         get
            {
            return this.nTabIndex;
            }
         set
            {
            this.nTabIndex = value;
            }
         }

      }
   }
