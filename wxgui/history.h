// This file is part of fityk program. Copyright 2009 Marcin Wojdyr
// Licence: GNU General Public License ver. 2+
/// SumHistoryDlg: a dialog for Fit > Parameter History

#ifndef FITYK_WX_HISTORY_H_
#define FITYK_WX_HISTORY_H_

#include <vector>
#include <wx/wx.h>
#include <wx/listctrl.h>

class wxSpinEvent;

class SumHistoryDlg : public wxDialog
{
public:
    SumHistoryDlg(wxWindow* parent, wxWindowID id);
private:
    int view[4], view_max;
    wxListView *lc;
    wxButton *compute_wssr_button;
    bool wssr_done; // flag to avoid calculation of wssr again

    void OnComputeWssrButton(wxCommandEvent&) { compute_all_wssr(); }
    void OnClearHistory(wxCommandEvent&) { clear_history(); }
    void OnSelectedItem(wxListEvent& event);
    void OnFocusedItem(wxListEvent& event);
    void OnViewSpinCtrlUpdate(wxSpinEvent& event);
    void compute_all_wssr();
    void clear_history();
    void OnActivate(wxActivateEvent&) { compute_all_wssr(); }

    void initialize_lc();
    DECLARE_EVENT_TABLE()
};

#endif // FITYK_WX_HISTORY_H_
