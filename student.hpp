#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <ostream>
#include <string>

/**
 * Data structure representing a student record.
 */
struct Student {
  std::string sid;
  std::string given_name;
  std::string family_name;
  std::string major;
  std::string description;
};


/**
 *  Stream a student record in a human-readable format.
 */
std::ostream &operator<<(std::ostream &os, const Student &student);

#endif // STUDENT_HPP