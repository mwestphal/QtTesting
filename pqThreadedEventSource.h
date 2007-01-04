/*=========================================================================

   Program: ParaView
   Module:    pqThreadedEventSource.h

   Copyright (c) 2005,2006 Sandia Corporation, Kitware Inc.
   All rights reserved.

   ParaView is a free software; you can redistribute it and/or modify it
   under the terms of the ParaView license version 1.1. 

   See License_v1.1.txt for the full ParaView license.
   A copy of this license can be obtained by contacting
   Kitware Inc.
   28 Corporate Drive
   Clifton Park, NY 12065
   USA

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef _pqThreadedEventSource_h
#define _pqThreadedEventSource_h

#include "QtTestingExport.h"
#include "pqEventSource.h"

class QString;
class QMutex;

/// Abstract interface for objects that can supply high-level testing events
/// on a separte thread.  This class is derived from, and run() is
/// implemented.
class QTTESTING_EXPORT pqThreadedEventSource : public pqEventSource
{
  Q_OBJECT
public:
  pqThreadedEventSource(QObject* p);
  ~pqThreadedEventSource();

  /** Called by the dispatcher on the GUI thread.
    Retrieves the next available event.  Returns true if an event was
    returned, false if there are no more events. 
    In the case of a threaded event source, this function is called by the GUI
    thread and waits for the other thread to pos and event. */
  virtual int getNextEvent(
    QString& object,
    QString& command,
    QString& arguments);
  
  /** The testing thread may post an event for the GUI to process.
      This function blocks until there are no previously queued 
      events to play.
      If the event plays successfully, true is returned. */
  bool postNextEvent(const QString& object,
                     const QString& command,
                     const QString& argument);

  /** tell this source to stop */
  void stop();

  /** Wait for the GUI thread to acknowledge an event.
      A previously locked mutex must be passed in.
      For use by the testing thread.
      If return value is false, an error occurred and 
      the testing thread should terminate. */
  bool waitForGUI(QMutex& m);

  /** Give the testing thread an acknowledgement.
      For use by the GUI thread */
  void guiAcknowledge();
  
private slots:

  void relayEvent(QString object, QString command, QString arguments);

protected:
  // start the thread
  void start();

  // called by the testing thread to signify it is done
  // pass in zero for success, non-zero for failure
  void done(int);

  // run the thread, return 
  virtual void run() = 0;

private:
  class pqInternal;
  pqInternal* Internal;

};

#endif // !_pqThreadedEventSource_h

