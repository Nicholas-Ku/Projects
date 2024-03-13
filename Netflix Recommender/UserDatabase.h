#ifndef USERDATABASE_INCLUDED
#define USERDATABASE_INCLUDED

#include <string>
#include <vector>

#include "treemm.h"

class User;

class UserDatabase
{
  public:
    UserDatabase();
    ~UserDatabase();
    bool load(const std::string& filename);
    User* get_user_from_email(const std::string& email) const;

  private:
      TreeMultimap<std::string, User*> m_tree;
      std::vector<User*> m_users;
      bool m_has_loaded;
      const int arbitrarily_large_number = 10000;
};

#endif // USERDATABASE_INCLUDED
