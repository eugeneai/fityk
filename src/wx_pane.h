// This file is part of fityk program. Copyright (C) Marcin Wojdyr
// $Id$

#ifndef WX_PANE__H__
#define WX_PANE__H__

#ifdef __GNUG__
#pragma interface
#endif

#include <wx/print.h>
#include <wx/config.h>
#include "wx_common.h"  //for Mouse_mode_enum, Output_style_enum
#include <wx/splitter.h>

class PlotPane;
class IOPane;
class MainPlot;
class DiffPlot;


// based on http://wiki.wxpython.org/index.cgi/ProportionalSplitterWindow
class ProportionalSplitter: public wxSplitterWindow
{
public:
    ProportionalSplitter(wxWindow* parent, 
                         wxWindowID id=-1, 
                         float proportion=0.66, // 0. - 1.
                         const wxSize& size = wxDefaultSize,
                         //IMHO default wxSP_3D doesn't look good (on wxGTK).
                         long style=wxSP_NOBORDER|wxSP_FULLSASH,
                         const wxString& name = "proportionalSplitterWindow");
    bool SplitHorizontally(wxWindow* win1, wxWindow* win2, float proportion=-1);
    bool SplitVertically(wxWindow* win1, wxWindow* win2, float proportion=-1);
    int GetExpectedSashPosition();
    void ResetSash();
    float GetProportion() { return m_proportion; }
    void SetProportion(float proportion) {m_proportion=proportion; ResetSash();}

protected:
    float m_proportion; //0-1
    bool m_firstpaint;

    void OnReSize(wxSizeEvent& event);
    void OnSashChanged(wxSplitterEvent &event);
    void OnPaint(wxPaintEvent &event);
};


class FCombo : public wxComboBox
{
public:
    FCombo(wxWindow *parent, wxWindowID id,
                const wxString& value = wxEmptyString,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                int n = 0, const wxString choices[] = NULL,
                long style = 0,
                const wxValidator& validator = wxDefaultValidator,
                const wxString& name = wxComboBoxNameStr)
    : wxComboBox(parent, id, value, pos, size, n, choices, style,
                    validator, name) { }
protected:
    void OnKeyDown (wxKeyEvent& event);

    DECLARE_EVENT_TABLE()
};


class Output_win : public wxTextCtrl
{
public:
    Output_win (wxWindow *parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition, 
                const wxSize& size = wxDefaultSize);
    void append_text (const wxString& str, Output_style_enum style);
    void OnRightDown (wxMouseEvent& event);
    void OnPopupColor  (wxCommandEvent& event);       
    void OnPopupFont   (wxCommandEvent& event);  
    void OnPopupClear  (wxCommandEvent& event); 
    void OnKeyDown     (wxKeyEvent& event);
    void save_settings(wxConfigBase *cf) const;
    void read_settings(wxConfigBase *cf);

private:
    wxColour text_color[4]; 
    wxColour bg_color;

    void fancy_dashes();

    DECLARE_EVENT_TABLE()
};


class IOPane : public wxPanel
{
public:
    IOPane(wxWindow *parent, wxWindowID id=-1);
    void append_text (const wxString& str, Output_style_enum style) 
                                      { output_win->append_text(str, style); }
    void save_settings(wxConfigBase *cf) const;
    void read_settings(wxConfigBase *cf);
private:
    Output_win *output_win;
    FCombo   *input_combo;

    DECLARE_EVENT_TABLE()
};


class PlotPane : public ProportionalSplitter
{
    friend class FPrintout;
public:
    PlotPane(wxWindow *parent, wxWindowID id=-1);
    void zoom_forward();
    std::string zoom_backward(int n=1);
    void save_settings(wxConfigBase *cf) const;
    void read_settings(wxConfigBase *cf);
    void refresh_plots(bool update);
private:
    Plot_shared plot_shared;
    MainPlot *plot;
    DiffPlot *diff_plot;
    std::vector<std::string> zoom_hist;

    DECLARE_EVENT_TABLE()
};


class DataPane : public wxPanel
{
public:
    DataPane(wxWindow *parent, wxWindowID id=-1);
private:

    DECLARE_EVENT_TABLE()
};



class FPrintout: public wxPrintout
{
public:
    FPrintout(const PlotPane *p_pane);
    bool HasPage(int page) { return (page == 1); }
    bool OnPrintPage(int page);
    void GetPageInfo(int *minPage,int *maxPage,int *selPageFrom,int *selPageTo)
        { *minPage = *maxPage = *selPageFrom = *selPageTo = 1; }
private:
    const PlotPane *pane;
};

//------------------------------------------------------------------

wxColour read_color_from_config(const wxConfigBase *config, const wxString& key,
                                const wxColour& default_value);

void write_color_to_config (wxConfigBase *config, const wxString& key,
                            const wxColour& value);
inline bool read_bool_from_config (const wxConfigBase *config, 
                                   const wxString& key, bool def_val)
                    { bool b; config->Read(key, &b, def_val); return b; }



#endif //WX_PANE__H__

