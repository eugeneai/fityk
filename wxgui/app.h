// This file is part of fityk program. Copyright 2001-2013 Marcin Wojdyr
// Licence: GNU General Public License ver. 2+

#ifndef FITYK_WX_APP_H_
#define FITYK_WX_APP_H_


class wxCmdLineParser;


/// Fityk-GUI "main loop"
class FApp: public wxApp
{
public:
    // directory for (named by user) config files
    wxString config_dir;

    FApp();
    virtual bool OnInit(void);
    virtual int OnExit();
#ifdef __WXMAC__
    virtual void MacOpenFile(const wxString &fileName);
#endif

private:
    bool is_fityk_script(std::string filename);
    void process_argv(wxCmdLineParser &cmdLineParser);
};

wxString get_help_url(const wxString& name);
wxString get_sample_path(const wxString& name);
#ifdef __WXMAC__
void open_new_instance();
#endif

DECLARE_APP(FApp)

#endif

