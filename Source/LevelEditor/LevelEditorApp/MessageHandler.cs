using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using System.Threading.Tasks;
using SDL2;

namespace LevelEditorApp
   {
   public class MessageHandler : IMessageFilter
      {
      const int WM_LBUTTONDOWN = 0x0201;
      const int WM_LBUTTONUP = 0x0202;
      const int WM_LBUTTONDBLCLK = 0x0203;
      const int WM_RBUTTONDOWN = 0x0204;
      const int WM_RBUTTONUP = 0x0205;
      const int WM_RBUTTONDBLCLK = 0x0206;
      const int WM_MBUTTONDOWN = 0x0207;
      const int WM_MBUTTONUP = 0x0208;
      const int WM_MBUTTONDBLCLK = 0x0209;

      const int WM_KEYDOWN = 0x0100;
      const int WM_KEYUP = 0x0101;
      const int WM_SYSKEYDOWN = 0x0104;
      const int WM_SYSKEYUP = 0x0105;
      const int WM_CLOSE = 0x0010;

      Panel m_DisplayPanel;
      Editor m_Parent;

      public MessageHandler( Panel displayPanel, Editor parent )
         {
         m_DisplayPanel = displayPanel;
         m_Parent = parent;
         }

      bool CheckFakeFocus()
         {
         System.Drawing.Point position = Cursor.Position;
         System.Drawing.Point relativeToForm = m_DisplayPanel.PointToClient( position );
         return ( relativeToForm.X >= 0 && relativeToForm.Y >= 0 &&
             relativeToForm.X < m_DisplayPanel.Width && relativeToForm.Y < m_DisplayPanel.Width );
         }

      public bool PreFilterMessage( ref Message m )
         {

         return false;
         }

      }
   }
