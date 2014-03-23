/**
 * PsmkContextMenu.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU Lesser General Public License (LGPL)
 * available at http://www.gnu.org/licenses/lgpl.html
 */

#ifndef PSMKCONTEXTMENU_HPP
#define PSMKCONTEXTMENU_HPP PSMKCONTEXTMENU_HPP

/* base class */
#include <QMenu>

/* system headers */

/* Qt headers */
#include <QString>
#include <QVariant>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QAction;
class QSettings;

/* forward declaration of local classes */

/*!
 \brief generic context menu subroutine

 used for all widget containing data:
 \ref PsmkBeatWidget
 \ref PsmkHiHatWidget
 \ref PsmkInstrumentsWidget
 \ref PsmkNoteWidget
 \ref PsmkPatternWidget

*/
class PsmkContextMenu : public QMenu
{
   Q_OBJECT
public:
   /*!
    \brief \todo

    \param context
    \param mimeType
    \param variantMap
    \param parent
   */
   explicit PsmkContextMenu( const QString &context,
                             const char *mimeType,
                             const QVariantMap &variantMap,
                             QWidget *parent = 0 );
   /*!
    \brief \todo

   */
   virtual ~PsmkContextMenu();

   /*!
    \brief \todo

   */
   enum SelectedAction
   {
      None,
      Copy,
      Paste
   };

   /*!
    \brief \todo

    \param pos
    \return SelectedAction
   */
   SelectedAction runSelect( const QPoint &pos );

   /*!
    \brief \todo

    \param data
   */
   QVariant mimeData() const;
   /*!
    \brief write mime data to clipboard

    \param data
   */
   void setMimeData( const QVariant &data );
   /*!
    \brief get mime data from clipboard

   */
   void getMimeData();
   /*!
    \brief \todo

   */
   void removeStore();
   /*!
    \brief \todo

    \param variantMap
    \param success
    \return
   */
   static QString getNameFromMap( const QVariantMap &variantMap, bool *success = 0 );

private:
   Q_DISABLE_COPY( PsmkContextMenu )

   QAction           *mpTitle; /*!< \brief \todo TODO */
   QAction           *mpSeparator1; /*!< \brief \todo TODO */
   QAction           *mpCopy; /*!< \brief \todo TODO */
   QAction           *mpPaste; /*!< \brief \todo TODO */
   QAction           *mpSeparator2; /*!< \brief \todo TODO */
   QAction           *mpStore; /*!< \brief \todo TODO */
   QMenu             *mpLoad; /*!< \brief \todo TODO */
   QMenu             *mpRemove; /*!< \brief \todo TODO */
   QSettings         *mpSettings; /*!< \brief \todo TODO */
   const char        *mpMimeType; /*!< \brief \todo TODO */
   QString           mName; /*!< \brief \todo TODO */
   QVariant          mData; /*!< \brief \todo TODO */
   QList<QAction*>   mLoadList; /*!< \brief \todo TODO */
   QList<QAction*>   mRemoveList; /*!< \brief \todo TODO */
};

#endif
