//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "hugglequeueitemlabel.hpp"
#include "ui_hugglequeueitemlabel.h"

using namespace Huggle;

int HuggleQueueItemLabel::Count = 0;

HuggleQueueItemLabel::HuggleQueueItemLabel(QWidget *parent) : QFrame(parent), ui(new Ui::HuggleQueueItemLabel)
{
    this->ParentQueue = (HuggleQueue*)parent;
    this->ui->setupUi(this);
}

HuggleQueueItemLabel::~HuggleQueueItemLabel()
{
    delete this->ui;
}

void HuggleQueueItemLabel::SetName(QString name)
{
    this->ui->label_2->setText(name);
    if (this->page != NULL)
    {
        // change the icon according to edit type
        if (this->page->OwnEdit)
        {
            this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-me.png"));
            return;
        }

        if (this->page->User->IsReported)
        {
            this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-reported.png"));
            return;
        }

        if (this->page->Bot)
        {
            this->ui->label->setPixmap( QPixmap(":/huggle/pictures/Resources/blob-bot.png") );
        }

        switch (this->page->CurrentUserWarningLevel)
        {
            case WarningLevelNone:
                this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-none.png"));
                break;
            case WarningLevel1:
                this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-warn-1.png"));
                return;
            case WarningLevel2:
                this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-warn-2.png"));
                return;
            case WarningLevel3:
                this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-warn-3.png"));
                return;
            case WarningLevel4:
                this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-warn-4.png"));
                return;
        }

        if (this->page->Score > 1000)
        {
            this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-warning.png"));
            return;
        }

        if (this->page->User->IsIP())
        {
            this->ui->label->setPixmap(QPixmap(":/huggle/pictures/Resources/blob-anon.png"));
            return;
        }
    }
}

QString HuggleQueueItemLabel::GetName()
{
    return this->ui->label_2->text();
}

void HuggleQueueItemLabel::Process(QLayoutItem *qi)
{
    HuggleQueueItemLabel::Count--;
    if (this->ParentQueue->Items.contains(this))
    {
        this->ParentQueue->Items.removeAll(this);
    }
    Core::HuggleCore->ProcessEdit(this->page);
    this->close();
    this->page->RegisterConsumer(HUGGLECONSUMER_MAINFORM);
    this->page->UnregisterConsumer(HUGGLECONSUMER_QUEUE);
    this->ParentQueue->Delete(this, qi);
}

void HuggleQueueItemLabel::Remove()
{
    HuggleQueueItemLabel::Count--;
    if (this->ParentQueue->Items.contains(this))
    {
        this->ParentQueue->Items.removeAll(this);
    }
    this->page->UnregisterConsumer(HUGGLECONSUMER_QUEUE);
    this->page = NULL;
    this->close();
    this->ParentQueue->Delete(this);
}

void HuggleQueueItemLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->Process();
        delete this;
    }
}
