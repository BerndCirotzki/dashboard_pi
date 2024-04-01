/******************************************************************************
 * $Id: speedometer.cpp, v1.0 2010/08/05 SethDart Exp $
 *
 * Project:  OpenCPN
 * Purpose:  Dashboard Plugin
 * Author:   Jean-Eudes Onfray
 *
 ***************************************************************************
 *   Copyright (C) 2010 by David S. Register   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301,  USA.             *
 ***************************************************************************
 */

#include "speedometer.h"
#include "wx/tokenzr.h"
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// Not much to do here most of the default dial values are fine.
// Note the default AngleStart = 225 and AngleRange = 270 set here.

DashboardInstrument_Speedometer::DashboardInstrument_Speedometer(
    wxWindow *parent, wxWindowID id, wxString title, InstrumentProperties* Properties, DASH_CAP cap_flag,
    int s_value, int e_value)
    : DashboardInstrument_Dial(parent, id, title, Properties, cap_flag, 225, 270, s_value,
                               e_value) {
  // We want the main value displayed inside the dial as well
  // as the default arrow
  SetOptionMainValue(_T("%.2f"), DIAL_POSITION_INSIDE);
}

DashboardInstrument_SpeedometerSOGSTW::DashboardInstrument_SpeedometerSOGSTW(
    wxWindow* parent, wxWindowID id, wxString title, InstrumentProperties* Properties, DASH_CAP cap_flag,
    int s_value, int e_value)
    : DashboardInstrument_Dial(parent, id, title, Properties, cap_flag, 225, 270, s_value,
        e_value) {
    // We want the main value displayed inside the dial as well
    // as the default arrow
    LastValue = 0;
    SetOptionMainValue(_T("%.2f"), DIAL_POSITION_INSIDE);
}

void DashboardInstrument_SpeedometerSOGSTW::SetData(DASH_CAP st, double data,
    wxString unit) {
    if (st == OCPN_DBP_STC_SOG) {
        m_MainValueTrue = data;
        m_MainValueTrueUnit = unit;
        m_MainValue = data;
        m_MainValueUnit = unit;
    }
    else if (st == OCPN_DBP_STC_STW) {
        m_MainValueApp = data;
        m_MainValueAppUnit = unit;
        m_ExtraValue = data;
        m_ExtraValueUnit = unit;
    }    
    Refresh();
}

void DashboardInstrument_SpeedometerSOGSTW::DrawData(wxGCDC* dc, double value, wxString unit,
    wxString format,
    DialPositionOption position) {

    wxColour cl;

    if (position == DIAL_POSITION_NONE) return;

    if (m_Properties)
    {
        dc->SetFont(m_Properties->m_LabelFont.GetChosenFont());
        if (position == DIAL_POSITION_BOTTOMLEFT)
            dc->SetTextForeground(GetColourSchemeFont(m_Properties->m_Arrow_Second_Colour));
        else if (position == DIAL_POSITION_BOTTOMRIGHT)
            dc->SetTextForeground(GetColourSchemeFont(m_Properties->m_Arrow_First_Colour));
        else
            dc->SetTextForeground(GetColourSchemeFont(m_Properties->m_LabelFont.GetColour()));
    }
    else
    {
        dc->SetFont(g_pFontLabel->GetChosenFont());
        if (position == DIAL_POSITION_BOTTOMLEFT)
        {
            GetGlobalColor(_T("BLUE3"), &cl);
            dc->SetTextForeground(cl);
        }
        else if (position == DIAL_POSITION_BOTTOMRIGHT)
        {
            GetGlobalColor(_T("DASHN"), &cl);
            dc->SetTextForeground(cl);
        }
        else
            dc->SetTextForeground(GetColourSchemeFont(g_pFontLabel->GetColour()));
    }
    wxSize size = GetClientSize();

    wxString text;
    if (!std::isnan(value)) {
        if (unit == _T("\u00B0"))
            text = wxString::Format(format, value) + DEGREE_SIGN;
        else if (unit == _T("\u00B0L"))  // No special display for now, might be
            // XX�< (as in text-only instrument)
            text = wxString::Format(format, value) + DEGREE_SIGN;
        else if (unit ==
            _T("\u00B0R"))  // No special display for now, might be >XX�
            text = wxString::Format(format, value) + DEGREE_SIGN;
        else if (unit == _T("\u00B0T"))
            text = wxString::Format(format, value) + DEGREE_SIGN + _T("T");
        else if (unit == _T("\u00B0M"))
            text = wxString::Format(format, value) + DEGREE_SIGN + _T("M");
        else if (unit == _T("N"))  // Knots
            text = wxString::Format(format, value) + _T(" Kts");
        else
            text = wxString::Format(format, value) + _T(" ") + unit;
    }
    else
        text = _T("---");

    int width, height;
    wxFont f;
    if (m_Properties)
        f = m_Properties->m_LabelFont.GetChosenFont();
    else
        f = g_pFontLabel->GetChosenFont();
    dc->GetMultiLineTextExtent(text, &width, &height, NULL, &f);

    wxRect TextPoint;
    TextPoint.width = width;
    TextPoint.height = height;
    switch (position) {
    case DIAL_POSITION_NONE:
        // This case was already handled before, it's here just
        // to avoid compiler warning.
        return;
    case DIAL_POSITION_INSIDE: {
        TextPoint.x = m_cx - (width / 2) - 1;
        TextPoint.y = (size.y * .75) - height;
        if (m_Properties)
            cl = GetColourSchemeBackgroundColour(m_Properties->m_TitlelBackgroundColour);
        else
            GetGlobalColor(_T("DASHL"), &cl);
        int penwidth = size.x / 100;
        wxPen* pen =
            wxThePenList->FindOrCreatePen(cl, penwidth, wxPENSTYLE_SOLID);
        dc->SetPen(*pen);
        if (m_Properties)
            cl = GetColourSchemeBackgroundColour(m_Properties->m_DataBackgroundColour);
        else
            GetGlobalColor(_T("DASHB"), &cl);
        dc->SetBrush(cl);
        // There might be a background drawn below
        // so we must clear it first.
        dc->DrawRoundedRectangle(TextPoint.x - 2, TextPoint.y - 2, width + 4,
            height + 4, 3);
        break;
    }
    case DIAL_POSITION_TOPLEFT:
        TextPoint.x = 0;
        TextPoint.y = m_TitleHeight;
        break;
    case DIAL_POSITION_TOPRIGHT:
        TextPoint.x = size.x - width - 1;
        TextPoint.y = m_TitleHeight;
        break;
    case DIAL_POSITION_BOTTOMLEFT:
        TextPoint.x = 0;
        TextPoint.y = size.y - height;
        break;
    case DIAL_POSITION_BOTTOMRIGHT:
        if (value >= 10 && LastValue < 10)
        {
            SetOptionExtraValue(OCPN_DBP_STC_STW, _T("     STW\n%.1f"), DIAL_POSITION_BOTTOMRIGHT);
            Refresh();
        }
        if (value < 10 && LastValue >= 10)
        {
            SetOptionExtraValue(OCPN_DBP_STC_STW, _T("   STW\n%.1f"), DIAL_POSITION_BOTTOMRIGHT);
            Refresh();
        }
        LastValue = value;
        TextPoint.x = size.x - width - 1;
        TextPoint.y = size.y - height;
        break;
    }

    //wxColour c2;
    //GetGlobalColor(_T("DASHB"), &c2);
    wxColour c3;
    GetGlobalColor(_T("DASHF"), &c3);

    wxStringTokenizer tkz(text, _T("\n"));
    wxString token;

    token = tkz.GetNextToken();
    while (token.Length()) {
        if (m_Properties)
            f = m_Properties->m_LabelFont.GetChosenFont();
        else
            f = g_pFontLabel->GetChosenFont();
        dc->GetTextExtent(token, &width, &height, NULL, NULL, &f);
        dc->DrawText(token, TextPoint.x, TextPoint.y);
        TextPoint.y += height;
        token = tkz.GetNextToken();
    }
}

void DashboardInstrument_SpeedometerSOGSTW::DrawForeground(wxGCDC* dc) {
    wxPoint points[4];
    double data;
    double val;
    double value;
    // The default foreground is the arrow used in most dials
    wxColour cl;
    GetGlobalColor(_T("DASH2"), &cl);
    wxPen pen1;
    pen1.SetStyle(wxPENSTYLE_SOLID);
    pen1.SetColour(cl);
    pen1.SetWidth(2);
    dc->SetPen(pen1);
    GetGlobalColor(_T("DASH1"), &cl);
    wxBrush brush1;
    brush1.SetStyle(wxBRUSHSTYLE_SOLID);
    brush1.SetColour(cl);
    dc->SetBrush(brush1);
    dc->DrawCircle(m_cx, m_cy, m_radius / 8);

    /*True Wind*/
    dc->SetPen(*wxTRANSPARENT_PEN);
    if (m_Properties)
        cl = GetColourSchemeFont(m_Properties->m_Arrow_Second_Colour);
    else
        GetGlobalColor(_T("BLUE3"), &cl);
    wxBrush brush2;
    brush2.SetStyle(wxBRUSHSTYLE_SOLID);
    brush2.SetColour(cl);
    dc->SetBrush(brush2);

    /* this is fix for a +/-180? round instrument, when m_MainValue is supplied as
     * <0..180><L | R> for example TWA & AWA */
    if (m_MainValueTrueUnit == _T("\u00B0L"))
        data = 360 - m_MainValueTrue;
    else
        data = m_MainValueTrue;

    // The arrow should stay inside fixed limits
    if (data < m_MainValueMin)
        val = m_MainValueMin;
    else if (data > m_MainValueMax)
        val = m_MainValueMax;
    else
        val = data;

    value = deg2rad((val - m_MainValueMin) * m_AngleRange /
        (m_MainValueMax - m_MainValueMin)) +
        deg2rad(m_AngleStart - ANGLE_OFFSET);

    points[0].x = m_cx + (m_radius * 0.95 * cos(value - .010));
    points[0].y = m_cy + (m_radius * 0.95 * sin(value - .010));
    points[1].x = m_cx + (m_radius * 0.95 * cos(value + .015));
    points[1].y = m_cy + (m_radius * 0.95 * sin(value + .015));
    points[2].x = m_cx + (m_radius * 0.22 * cos(value + 2.8));
    points[2].y = m_cy + (m_radius * 0.22 * sin(value + 2.8));
    points[3].x = m_cx + (m_radius * 0.22 * cos(value - 2.8));
    points[3].y = m_cy + (m_radius * 0.22 * sin(value - 2.8));
    dc->DrawPolygon(4, points, 0, 0);

    /* Apparent Wind*/
    dc->SetPen(*wxTRANSPARENT_PEN);
    if (m_Properties)
        cl = GetColourSchemeFont(m_Properties->m_Arrow_First_Colour);
    else
        GetGlobalColor(_T("DASHN"), &cl);
    wxBrush brush;
    brush.SetStyle(wxBRUSHSTYLE_SOLID);
    brush.SetColour(cl);
    dc->SetBrush(brush);

    /* this is fix for a +/-180? round instrument, when m_MainValue is supplied as
     * <0..180><L | R> for example TWA & AWA */
    if (m_MainValueAppUnit == _T("\u00B0L"))
        data = 360 - m_MainValueApp;
    else
        data = m_MainValueApp;

    // The arrow should stay inside fixed limits
    if (data < m_MainValueMin)
        val = m_MainValueMin;
    else if (data > m_MainValueMax)
        val = m_MainValueMax;
    else
        val = data;

    value = deg2rad((val - m_MainValueMin) * m_AngleRange /
        (m_MainValueMax - m_MainValueMin)) +
        deg2rad(m_AngleStart - ANGLE_OFFSET);

    points[0].x = m_cx + (m_radius * 0.95 * cos(value - .010));
    points[0].y = m_cy + (m_radius * 0.95 * sin(value - .010));
    points[1].x = m_cx + (m_radius * 0.95 * cos(value + .015));
    points[1].y = m_cy + (m_radius * 0.95 * sin(value + .015));
    points[2].x = m_cx + (m_radius * 0.22 * cos(value + 2.8));
    points[2].y = m_cy + (m_radius * 0.22 * sin(value + 2.8));
    points[3].x = m_cx + (m_radius * 0.22 * cos(value - 2.8));
    points[3].y = m_cy + (m_radius * 0.22 * sin(value - 2.8));
    dc->DrawPolygon(4, points, 0, 0);
}
