/******************************************************************************
 *
 * Copyright (c) 2017 Antillia.com TOSHIYUKI ARAI. ALL RIGHTS RESERVED.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 *      ErrorDialog.h
 *
 *****************************************************************************/

#pragma once

#include <glibmm/ustring.h>
#include <gtkmm.h>

namespace Es {
  
class ErrorDialog :public Gtk::MessageDialog {

public:
  //ErrorDialog(Gtk::Window& owner,  const std::string& first_label)
  ErrorDialog(Gtk::Window& owner,  const Glib::ustring& first_label)
    :MessageDialog(owner, first_label) 
  {
    set_title("Exception");
  }

  //static members 
  static Es::ErrorDialog* errorDialog;

  static void createErrorDialog(Gtk::Window& owner)
  {
    if (errorDialog == NULL) {
     //const std::string text = "Error:";
     const Glib::ustring text = "Error:";
     errorDialog = new ErrorDialog(owner, text);
    }
  }

public:
  static void show(const std::string& secondary_text)
  //static void show(std::string& secondary_text)
  {
    if (errorDialog) {
      errorDialog->set_secondary_text(secondary_text);
      errorDialog->run();
      errorDialog->hide();
    }
  }
};
//Global variable:
  Es::ErrorDialog* ErrorDialog::errorDialog = NULL;
}


