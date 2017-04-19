/* - Range ID Convertor - Copyright (C) 2017 Sebastien Vavassori
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the MIT License.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "exporter.h"

#include <QtCore/QDebug>
#include <QtWidgets/QComboBox>


/*!
 * \class Exporter
 * \brief The Exporter class formats an input text to a specific output format.
 *
 * Exporter is a decorator.
 *
 */

Exporter::Exporter(QObject *parent) : QObject(parent)
  , m_choice(ChoiceType::CALC_COLUMN)
{
    this->populate();
}

Exporter::~Exporter()
{
}

/***********************************************************************************
 ***********************************************************************************/
/*! \brief Connect the given \a combobox to the Exporter.
 * It populates the given QComboBox with the export choices.
 */
void Exporter::connectComboBox(QComboBox *combobox)
{
    combobox->clear();
    //combobox->addItem(tr("<Select a format in the list>"));
    foreach (auto v, m_choices) {
        combobox->addItem(v.text);
    }
    //combobox->addItem(tr("---"));

    combobox->setCurrentText( toString(m_choice) );
    connect(combobox, SIGNAL(currentTextChanged(QString)), this, SLOT(setChoice(QString)));
    connect(this, SIGNAL(choiceChanged(QString)), combobox, SLOT(setCurrentText(QString)));
}

/***********************************************************************************
 ***********************************************************************************/
QString Exporter::decorate(const QStringList &data) const
{
    QString ret;

    const int count = data.count();
    switch (m_choice) {

    case ChoiceType::CALC_LINE_COMMA :

        for (int i = 0; i < count; ++i) {
            ret += data.at(i);
            if (i < count - 1)
                ret += QStringLiteral(",");
        }
        ret += QStringLiteral("\n");
        break;

    case ChoiceType::CALC_LINE_TAB   :

        for (int i = 0; i < count; ++i) {
            ret += data.at(i);
            if (i < count - 1)
                ret += QStringLiteral("\t");
        }
        ret += QStringLiteral("\n");
        break;

    case ChoiceType::CALC_COLUMN     :

        foreach (auto line, data) {
            ret += line;
            ret += QStringLiteral("\n");
        }
        break;

    case ChoiceType::NASTRAN         :
    {
        int lineLength = 8;
        ret += QStringLiteral("$\n");
        ret += QStringLiteral("SET 1 = "); /* 8 chars */

        for (int i = 0; i < count; ++i) {
            const QString line = data.at(i);
            if ( (lineLength + line.length() + 1) > 70) {  /* 70 chars max */
                ret += QStringLiteral("\n");
                ret += QStringLiteral("        "); /* 8 chars */
                lineLength = 8;
            }
            ret += line;
            if (i < count - 1) {
                ret += QStringLiteral(",");
                lineLength += line.length() + 1;
            }
        }
        ret += QStringLiteral("\n");
        ret += QStringLiteral("$\n");
    }
        break;

    case ChoiceType::PATRAN_ELEMENTS :

        ret += QStringLiteral("Elm ");
        for (int i = 0; i < count; ++i) {
            ret += data.at(i);
            if (i < count - 1)
                ret += QStringLiteral(" ");
        }
        break;

    case ChoiceType::PATRAN_SES      :
    {
        int lineLength = 40;
        ret += QStringLiteral("# ############################## \n");
        ret += QStringLiteral("#     Patran Session file        \n");
        ret += QStringLiteral("# ############################## \n");
        ret += QStringLiteral("ga_group_create( \"group\" )\n");
        ret += QStringLiteral("ga_group_entity_add( \"group\", \" Element "); /* 40 chars */

        for (int i = 0; i < count; ++i) {
            const QString line = data.at(i);

            if (line.count(QLatin1Char(':')) > 1){
                /* ****************************************************** */
                /* Stops the process, because the format is not adapted.  */
                /* Example: "32002012:32002098:20"                        */
                /*                            ^^^                         */
                /*                             Cannot convert if packed!  */
                /* ****************************************************** */
                const QString s = data.at(i);
                const int position = s.indexOf(QLatin1Char(':'), 0);
                const int length = s.length();
                QString s1;
                QString s2;
                s1.fill(' ', position);
                s2.fill('^', length-position);

                ret.clear();
                ret += tr("Not implemented, sorry.\n");
                ret += tr("The item is not in an exportable format for Patran Session file:\n");
                ret += tr("'%0'\n").arg( data.at(i) );
                ret += tr(" %0%1\n").arg( s1 ).arg( s2 );
                ret += tr("--> contains two ':'\n");
                ret += tr("Recommendation: Choose the 'unpacked' format, and retry.\n");
                return ret;
            }

            if ( (lineLength + line.length() + 8) > 70) {  /* 70 chars max */
                ret += QStringLiteral("\" // @\n");
                ret += QStringLiteral("\"");  /* 1 char */
                lineLength = 1;
            }
            ret += line;
            if (i < count - 1) {
                ret += QStringLiteral(" ");
                lineLength += line.length() + 1;
            }
        }

        ret += QStringLiteral(" \" )\n");
        ret += QStringLiteral("#\n");

    }
        break;

    case ChoiceType::FEMAP           :

        for (int i = 0; i < count; ++i) {
            ret += data.at(i);
            if (i < count - 1)
                ret += QStringLiteral(",");
        }
        break;

    case ChoiceType::HYPERMESH       :

        ret += QStringLiteral("el ");

        for (int i = 0; i < count; ++i) {
            QString line = data.at(i);

            if (line.count(QLatin1Char(':')) > 1){
                /* ****************************************************** */
                /* Stops the process, because the format is not adapted.  */
                /* Example: "32002012:32002098:20"                        */
                /*                            ^^^                         */
                /*                             Cannot convert if packed!  */
                /* ****************************************************** */
                const QString s = data.at(i);
                const int position = s.indexOf(QLatin1Char(':'), 0);
                const int length = s.length();
                QString s1;
                QString s2;
                s1.fill(' ', position);
                s2.fill('^', length-position);

                ret.clear();
                ret += tr("Not implemented, sorry.\n");
                ret += tr("The item is not in an exportable format for HyperMesh:\n");
                ret += tr("'%0'\n").arg( data.at(i) );
                ret += tr(" %0%1\n").arg( s1 ).arg( s2 );
                ret += tr("--> contains two ':'\n");
                ret += tr("Recommendation: Choose the 'unpacked' format, and retry.\n");
                return ret;
            }

            line.replace(QLatin1Char(':'), QLatin1Char('-'));
            ret += line;
            if (i < count - 1)
                ret += QStringLiteral(",");

        }
        break;

    case ChoiceType::LAST_CHOICE     :
    default:
        ret += tr("Not implemented, sorry.\n");
        break;
    }


    return ret;
}


/***********************************************************************************
 ***********************************************************************************/
QString Exporter::choice() const
{
    return toString(m_choice);
}

void Exporter::setChoice(const QString &text)
{
    ChoiceType choice = toChoice(text);

    if (choice == m_choice)
        return;

    m_choice = choice;
    emit choiceChanged(text);
}

/***********************************************************************************
 ***********************************************************************************/
QString Exporter::toString(const Exporter::ChoiceType choice) const
{
    Q_ASSERT((int)choice >= 0);
    Q_ASSERT((int)choice < (int)ChoiceType::LAST_CHOICE);
    foreach (auto v, m_choices) {
        if (v.choice == choice) {
            return v.text;
        }
    }
    Q_UNREACHABLE();
    return QString();
}

Exporter::ChoiceType Exporter::toChoice(const QString &text) const
{
    foreach (auto v, m_choices) {
        if (v.text == text) {
            return v.choice;
        }
    }
    return ChoiceType::LAST_CHOICE;
}

void Exporter::populate()
{
    m_choices.append(Choice(CALC_LINE_COMMA , tr("Calc/Excel (as line, comma-separated)")));
    m_choices.append(Choice(CALC_LINE_TAB   , tr("Calc/Excel (as line, tab-separated)")));
    m_choices.append(Choice(CALC_COLUMN     , tr("Calc/Excel (as column)")));
    m_choices.append(Choice(NASTRAN         , tr("Nastran Bulk file (comma-separated)")));
    m_choices.append(Choice(PATRAN_ELEMENTS , tr("Patran (as elements)")));
    m_choices.append(Choice(PATRAN_SES      , tr("Patran Session file (as elements)")));
    m_choices.append(Choice(FEMAP           , tr("Femap (as elements)")));
    m_choices.append(Choice(HYPERMESH       , tr("HyperMesh (as elements)")));

    Q_ASSERT(m_choices.count() == (int)ChoiceType::LAST_CHOICE - 1);
}
