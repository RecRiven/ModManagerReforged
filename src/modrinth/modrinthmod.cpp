#include "modrinthmod.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <memory>

#include "modrinthapi.h"
#include "util/funcutil.h"

ModrinthMod::ModrinthMod(QObject *parent, const ModrinthModInfo &info) :
    QObject(parent),
    modInfo(info)
{

}

ModrinthModInfo ModrinthMod::getModInfo() const
{
    return modInfo;
}

void ModrinthMod::acquireIcon()
{
    if(modInfo.iconUrl.isEmpty() || gettingIcon) return;
    gettingIcon = true;
    QNetworkRequest request(modInfo.iconUrl);
    auto reply = accessManager()->get(request);
    connect(reply, &QNetworkReply::finished, this, [=]{
        gettingIcon = false;
        if(reply->error() != QNetworkReply::NoError) return;
        modInfo.iconBytes = reply->readAll();
        if(!modInfo.iconBytes.isEmpty())
            emit iconReady();
        reply->deleteLater();
    });
}

void ModrinthMod::acquireFullInfo()
{
    if(gettingFullInfo) return;
    gettingFullInfo = true;
    ModrinthAPI::getInfo(modInfo.modId, [=](const auto &newInfo){
        gettingFullInfo = false;
        if(modInfo.basicInfo){
            modInfo.description = newInfo.description;
            modInfo.versionList = newInfo.versionList;
        } else
            modInfo = newInfo;
        emit fullInfoReady();
    });
}

void ModrinthMod::acquireFileList()
{
    if(gettingFileList) return;
    gettingFileList = true;

    auto count = std::make_shared<int>(modInfo.versionList.size());

    for(const auto &version : qAsConst(modInfo.versionList)){
        ModrinthAPI::getVersion(version, [=](const auto &file){
            modInfo.fileList << file;
            (*count)--;
            if(*count == 0)
                emit fileListReady();
        });
    }
}
