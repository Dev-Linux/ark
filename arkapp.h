//  -*-C++-*-           emacs magic for .h files

#ifndef __ARKAPP_H__
#define __ARKAPP_H__

#include <qstring.h>
#include <kuniqueapp.h>
#include "arkwidget.h"

class ArkSettings;

#include <stdlib.h>
#include <string.h>
#include <hash_map>

class EqualKey
{
public:
  bool operator()(const QString & str1, const QString & str2) const
    {
      return (str1 == str2);
    }
};

// defines HashTable for obtaining ArkWidget pointers given a filename
typedef hash_map<QString, ArkWidget *, hash<char*>,EqualKey> HashTable;

// This class follows the singleton pattern.

class ArkApplication : public KUniqueApplication
{
  Q_OBJECT
public:
  virtual int newInstance();
  virtual ~ArkApplication() {}

  // keep track of windows so we know when to quit
  int windowCount() { return m_windowCount; }
  void addWindow() { ++m_windowCount; }
  void removeWindow() { --m_windowCount;} 

  // keep track of open archive names so we don't open one twice
  // note that ArkWidget is not a pointer to const because raise()
  // requires later a pointer to nonconst.
  void addOpenArk(const QString & _arkname, ArkWidget * _ptr);
  void removeOpenArk(const QString & _arkname);

  // returns -1 if ark is not already open; otherwise returns index
  bool isArkOpenAlready(const QString & _arkname)
    {  return (openArksList.findIndex(_arkname) != -1); }

  void raiseArk(const QString & _arkname);

  // use this function to access data from other modules.
  static ArkApplication *getInstance();
protected:
  ArkApplication();
private:
  QWidget *m_mainwidget;  // to be the parent of all ArkWidgets
  int m_windowCount;

  QStringList openArksList;

  // a hash to obtain the window associated with a filename.
  // given a QString key, you get an ArkWidget * pointer.
  HashTable m_windowsHash;

  static ArkApplication *mInstance; 
};

#endif // __ARKAPP_H__
