/*
 * Copyright (c) 2005-2014, BearWare.dk
 * 
 * Contact Information:
 *
 * Bjoern D. Rasmussen
 * Skanderborgvej 40 4-2
 * DK-8000 Aarhus C
 * Denmark
 * Email: contact@bearware.dk
 * Phone: +45 20 20 54 59
 * Web: http://www.bearware.dk
 *
 * This source code is part of the TeamTalk 5 SDK owned by
 * BearWare.dk. All copyright statements may not be removed 
 * or altered from any source distribution. If you use this
 * software in a product, an acknowledgment in the product 
 * documentation is required.
 *
 */

#include "uservolumedlg.h"
#include "appinfo.h"
#include "common.h"

#include <QMessageBox>

extern TTInstance* ttInst;

UserVolumeDlg::UserVolumeDlg(int userid, QWidget * parent/* = 0*/)
: QDialog(parent, QT_DEFAULT_DIALOG_HINTS)
, m_userid(userid)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(APPICON));

    ui.voicevolSlider->setRange(SOUND_VOLUME_MIN, DEFAULT_SOUND_VOLUME_MAX);
    ui.voicevolSlider->setSingleStep(VOLUME_SINGLE_STEP);
    ui.voicevolSlider->setPageStep(VOLUME_PAGE_STEP);
    ui.mfvolSlider->setRange(SOUND_VOLUME_MIN, DEFAULT_SOUND_VOLUME_MAX);
    ui.mfvolSlider->setSingleStep(VOLUME_SINGLE_STEP);
    ui.mfvolSlider->setPageStep(VOLUME_PAGE_STEP);

    connect(ui.voicevolSlider, SIGNAL(valueChanged(int)),
            SLOT(slotVolumeChanged(int)));
    connect(ui.voiceleftChkBox, SIGNAL(clicked()), SLOT(slotMuteChannel()));
    connect(ui.voicerightChkBox, SIGNAL(clicked()), SLOT(slotMuteChannel()));

    connect(ui.mfvolSlider, SIGNAL(valueChanged(int)),
            SLOT(slotVolumeChanged(int)));
    connect(ui.mfleftChkBox, SIGNAL(clicked()), SLOT(slotMuteChannel()));
    connect(ui.mfrightChkBox, SIGNAL(clicked()), SLOT(slotMuteChannel()));
    connect(ui.defaultsButton, SIGNAL(clicked()), SLOT(slotDefaults()));

    User user;
    if(TT_GetUser(ttInst, m_userid, &user))
        setWindowTitle(windowTitle() + QString(" - ") + _Q(user.szNickname));
    ui.voicevolSlider->setValue(user.nVolumeVoice);
    ui.mfvolSlider->setValue(user.nVolumeMediaFile);

    ui.voiceleftChkBox->setChecked(!user.stereoPlaybackVoice[0]);
    ui.voicerightChkBox->setChecked(!user.stereoPlaybackVoice[1]);
    ui.mfleftChkBox->setChecked(!user.stereoPlaybackMediaFile[0]);
    ui.mfrightChkBox->setChecked(!user.stereoPlaybackMediaFile[1]);
}

void UserVolumeDlg::slotVolumeChanged(int /*vol*/)
{
    BOOL b = TRUE;
    b &= TT_SetUserVolume(ttInst, m_userid, STREAMTYPE_VOICE,
                          ui.voicevolSlider->value());
    b &= TT_SetUserVolume(ttInst, m_userid, STREAMTYPE_MEDIAFILE_AUDIO,
                          ui.mfvolSlider->value());

    if(!b)
        QMessageBox::critical(this, tr("Volume"), 
        tr("Failed to change user's volume"));
}

void UserVolumeDlg::slotMuteChannel()
{
    TT_SetUserStereo(ttInst, m_userid,
                     STREAMTYPE_VOICE,
                     !ui.voiceleftChkBox->isChecked(), 
                     !ui.voicerightChkBox->isChecked());
    TT_SetUserStereo(ttInst, m_userid,
                     STREAMTYPE_MEDIAFILE_AUDIO,
                     !ui.mfleftChkBox->isChecked(), 
                     !ui.mfrightChkBox->isChecked());
}

void UserVolumeDlg::slotDefaults()
{
    ui.voicevolSlider->setValue(SOUND_VOLUME_DEFAULT);
    ui.mfvolSlider->setValue(SOUND_VOLUME_DEFAULT);
    slotVolumeChanged(SOUND_VOLUME_DEFAULT);
}

