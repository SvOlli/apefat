/*
 * VoiceModel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef VOICEMODEL_HPP
#define VOICEMODEL_HPP VOICEMODEL_HPP

/* base class */
#include <QAbstractListModel>

/* system headers */

/* Qt headers */
#include <QString>

/* local library headers */

/* local headers */
#include "SlocumSong.hpp"

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
 \brief \todo complete documentation

*/
class VoiceModel : public QAbstractListModel
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param slocumSong
    \param voice
    \param parent
   */
   explicit VoiceModel( SlocumSong *slocumSong, int voice, QObject *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~VoiceModel();

   /*!
    \brief get number of elements in list

    \param parent
    \return int
   */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;

   /*!
    \brief get data for display, reimplemented to match model requirements

    \param index
    \param role
    \return QVariant
   */
   QVariant data( const QModelIndex &index, int role ) const;
   /*!
    \brief set the data

    \param index
    \param value
    \param role
    \return bool
   */
   bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

   /*!
    \brief reimplemented to match model requirements

    \param index
    \return Qt::ItemFlags
   */
   Qt::ItemFlags flags( const QModelIndex &index ) const;
   /*!
    \brief insert empty rows

    \param row
    \param count
    \param parent
    \return bool
   */
   bool insertRows( int row, int count, const QModelIndex &parent = QModelIndex() );
   /*!
    \brief remove rows and drop their data

    \param row
    \param count
    \param parent
    \return bool
   */
   bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );
   /*!
    \brief set the model data

    \param SlocumSong
   */
   void setSlocumSong( SlocumSong *SlocumSong );
   /*!
    \brief reimplemented to match model requirements

    \return QStringList
   */
   QStringList mimeTypes() const;
   /*!
    \brief reimplemented for drag'n'drop

    \return Qt::DropActions
   */
   Qt::DropActions supportedDragActions() const;
   /*!
    \brief reimplemented for drag'n'drop

    \return Qt::DropActions
   */
   Qt::DropActions supportedDropActions() const;
   /*!
    \brief get the mime data, reimplemented to match model requirements

    \param indexes
    \return QMimeData
   */
   QMimeData *mimeData( const QModelIndexList &indexes ) const;
   /*!
    \brief handle mime data of dropped objects

    \param data
    \param action
    \param row
    \param column
    \param parent
    \return bool
   */
   bool dropMimeData( const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent );

public slots:

private:
   Q_DISABLE_COPY( VoiceModel )

   SlocumSong     *mpSlocumSong; /*!< \brief \todo TODO */
   const int      mVoice; /*!< \brief \todo TODO */
};

#endif
