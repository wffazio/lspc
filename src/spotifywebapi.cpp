#include "inc/spotifywebapi.h"
#include "inc/playercontrols.h"

#include <QWidget>
#include <QDebug>
#include <QNetworkReply>
#include <QString>
#include <QJsonDocument>


/*---------------------------------------------------------------------------*/
SpotifyWebApi::SpotifyWebApi(QWidget *parent)
{
    connect(&networkAccessManager_,
            &QNetworkAccessManager::finished,
            this,
            &SpotifyWebApi::onNetworkReplyReceived_);

    connect(&statusTimer_,
            &QTimer::timeout,
            this,
            &SpotifyWebApi::requestServerStatus_);
    statusTimer_.setInterval(500);

}


/*---------------------------------------------------------------------------*/
void SpotifyWebApi::playSlot()
{
    qDebug() << "Playing";
    SpotifyWebApiSearch("Sepultura",SpotifyWebApiRequestType::spotifyWebApiRequestTypeTrackSearch);
}


/*---------------------------------------------------------------------------*/
void SpotifyWebApi::onNetworkReplyReceived_(QNetworkReply *reply)
{
    auto requestIterator = sentRequests_.find(reply);

    if (requestIterator != sentRequests_.end())
    {
        SpotifyWebApiRequest request = requestIterator.value();
        QByteArray replyData = reply->readAll();

        if (reply->error() == QNetworkReply::NoError)
        {
            switch (request.RequestType)
            {
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypeAction:
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypeAlbumSearch:
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypeTrackSearch:
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypeArtistSearch:
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypePlay:
                case SpotifyWebApiRequestType::spotifyWebApiRequestTypeServerStatus:
                default:
                QJsonParseError jsonParseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(static_cast<QString>(replyData).toUtf8(), &jsonParseError);

                if (jsonParseError.error != QJsonParseError::NoError)
                    return ;

                if (!jsonResponse.isObject()) return ;
                auto data = jsonResponse.toVariant().toMap();
                auto itemMap = data.value("tracks").toMap().value("items");

                if(itemMap.isNull()) return ;
                QList<QVariant> items = itemMap.toList();

                if(items.isEmpty())  return ;

                for (int i=0;items.count() > i;++i)
                {
                    auto item = items[i].toMap();
                    QString trackname = item.value("name").toString();
                    QString band = item.value("artists").toList().first().toMap().value("name").toString();
                    QString album = item.value("album").toMap().value("name").toString();
                    QString preview = item.value("preview_url").toString();
                    qDebug().noquote() <<"Faixa" << i << trackname << album << band << preview;
                }

                   // Ignore, we don't care.
                    break;
            }
        }
        else
        {
#if 0
            emit networkError(reply->error(), reply->url());

            // If Spotify is not running anymore, inform the user.
            if (   (NetworkRequestType_e::eNetworkRequest_ServerStatus == request.RequestType)
                && (QNetworkReply::ConnectionRefusedError == reply->error())
                && m_data->Online)
            {
                m_data->Online  = false;
                emit disconnected();
            }
#endif
        }

        // Clear the pending reply.
        sentRequests_.erase(requestIterator);

    }
    // Else the response is for another instance of the server. Ignore it.

    reply->deleteLater();
}


/*---------------------------------------------------------------------------*/
void SpotifyWebApi::requestServerStatus_()
{
    /*nothing for now*/
}


/*---------------------------------------------------------------------------*/
bool SpotifyWebApi::SpotifyWebApiSearch(QString what,  SpotifyWebApiRequestType type)
{
    QString urlString(SPOTIFY_API_BASE_URL SPOTIFY_API_VER SPOTIFY_API_SEARCH);
    urlString.append(what);

    switch(type)
    {
        case SpotifyWebApiRequestType::spotifyWebApiRequestTypeArtistSearch:
            urlString.append("&" SPOTIFY_API_SEARCH_ARTIST);
            break;
        case SpotifyWebApiRequestType::spotifyWebApiRequestTypeTrackSearch:
            urlString.append("&" SPOTIFY_API_SEARCH_TRACK);
            break;
        case SpotifyWebApiRequestType::spotifyWebApiRequestTypeAlbumSearch:
            urlString.append("&" SPOTIFY_API_SEARCH_ALBUM);
            break;
        default:
            qDebug() << "Unknow Request Type: " << type;
            return false;
    }
    if (token_==nullptr  || token_->isNull() || token_->isEmpty())
    {
        qDebug() << "No Token to make Search";
        return false;
    }
    QUrl url = QUrl(urlString);
    QNetworkRequest request = QNetworkRequest(QUrl(urlString));
    request.setRawHeader(tr("Authorization").toLocal8Bit(),tr("Bearer ").toLocal8Bit()+token_->toLocal8Bit());
    SpotifyWebApiRequest apiReq = {type,request};
    queueRequest_( apiReq);

    return true;
}


/*---------------------------------------------------------------------------*/
void SpotifyWebApi::queueRequest_(const SpotifyWebApiRequest &request)
{
    qDebug() << "Requesting: " << request.Request.url();
    sentRequests_[networkAccessManager_.get(request.Request)] = request;
}

/*---------------------------------------------------------------------------*/
void SpotifyWebApi::storeToken(QString token)
{
    qDebug() << "Storing tpken " << token << " to use with  api";
    token_=new QString(token);
}


