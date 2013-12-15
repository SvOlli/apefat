/*
 * src/Partyman/TrackInfoListModel.hpp
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
   /**
    * @brief constructor
    *
    * @param database
    * @param parent
    */
   explicit VoiceModel( SlocumSong *slocumSong, int voice, QObject *parent = 0 );
   /**
    * @brief destructor
    *
    */
   virtual ~VoiceModel();

   /*!
    \brief get number of elements in list

   */
   int rowCount( const QModelIndex &parent = QModelIndex() ) const;

   /*!
    \brief get data for display, reimplemented to match model requirements

   */
   QVariant data( const QModelIndex &index, int role ) const;
   /*!
    \brief set the data

   */
   bool setData( const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole );

   /*!
    \brief reimplemented to match model requirements

   */
   Qt::ItemFlags flags( const QModelIndex &index ) const;

   /*!
    \brief insert empty rows

   */
   bool insertRows( int row, int count, const QModelIndex &parent = QModelIndex() );
   /*!
    \brief remove rows and drop their data

   */
   bool removeRows( int row, int count, const QModelIndex &parent = QModelIndex() );

   /*!
    \brief set the model data

   */
   void setSlocumSong( SlocumSong *SlocumSong );

   /*!
    \brief reimplemented to match model requirements

   */
   QStringList mimeTypes() const;
   /*!
    \brief reimplemented for drag'n'drop

   */
   Qt::DropActions supportedDragActions() const;
   /*!
    \brief reimplemented for drag'n'drop

   */
   Qt::DropActions supportedDropActions() const;
   /*!
    \brief get the mime data, reimplemented to match model requirements

   */
   QMimeData *mimeData( const QModelIndexList &indexes ) const;
   /*!
    \brief handle mime data of dropped objects

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
