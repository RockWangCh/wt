/*
 * Copyright (C) 2010 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "UserAccount.h"
#include "Entry.h"

#include <Wt/WApplication>
#include <Wt/WLogger>

#include <Wt/Dbo/WtSqlTraits>

using namespace Wt;
using namespace Wt::Dbo;

UserAccount::UserAccount()
{ }

UserAccount::UserAccount(const WString& aName)
  : name(aName)
{ }

collection< ptr<Entry> > UserAccount::entriesInRange(const WDate& from, 
						     const WDate& until) const
{
  return entries.find()
    .where("start >= ?").bind(WDateTime(from))
    .where("start < ?").bind(WDateTime(until));
}

ptr<UserAccount> UserAccount::login(Session& session, 
				    const WString& userName)
{
  Transaction transaction(session);

  ptr<UserAccount> ua = 
    session.find<UserAccount>("where name = ?").bind(userName);

  if (!ua) {
    WApplication::instance()
      ->log("notice") << "Creating user: " << userName.toUTF8();

    ua = session.add(new UserAccount(userName));
  }

  transaction.commit(); 
 
  return ua;
}
