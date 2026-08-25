#include "system.hpp"

#include <cctype>
#include <vector>

/**
 * Convert a string to uppercase for case-insensitive indexing.
 */
std::string normalize_uppercase(const std::string &text) {
  std::string normalized = text;
  for (int i = 0; i < normalized.size(); ++i) {
    normalized[i] = static_cast<char>(
        std::toupper(static_cast<unsigned char>(normalized[i])));
  }
  return normalized;
}

/**
 * Create a vector of const Student * from a vector of Student *.
 */
std::vector<const Student *>
to_const_students(const std::vector<Student *> &students) {
  std::vector<const Student *> result;
  for (int i = 0; i < students.size(); ++i) {
    result.push_back(students[i]);
  }
  return result;
}

void update_except_id(Trie<std::vector<Student*>, 'A', 'Z'> &index, const std::string &key, Student* ptr){
  std::vector<Student*>* exist_vector = index.search(key);
  if(exist_vector != nullptr){
    exist_vector->push_back(ptr);
  }
  else{
    std::vector<Student *> new_stu_array;
    new_stu_array.push_back(ptr);
    index.insert(key, new_stu_array);
  }


}

System::System(){
}

System::~System(){
  for(Student* ptr : students){
    delete ptr;
  }
}

void System::insert(const Student &student){
  if(sid_index.search(student.sid) != nullptr){
    throw std::runtime_error("student already in system");
  }

  Student* new_student = new Student(student);

  students.push_back(new_student);
  sid_index.insert(new_student->sid, new_student);

  std::string student_name = normalize_uppercase(new_student->given_name);
  std::string student_family_name = normalize_uppercase(new_student->family_name);

  update_except_id(given_name_index, student_name, new_student);
  update_except_id(family_name_index, student_family_name, new_student);
  update_except_id(major_index, new_student->major, new_student);


}

const Student* System::lookup_sid(const std::string &sid) const{
  Student* const* check = sid_index.search(sid);
  
  if(check == nullptr){return nullptr;}
  else{
    return *check;
  }


}

std::vector<const Student *>
System::lookup_given_name(const std::string &given_name) const{
  std::string upper_name = normalize_uppercase(given_name);
  const std::vector<Student*>* check = given_name_index.search(upper_name);

  std::vector<const Student *> blank;
  if(check == nullptr){return blank;}

  return to_const_students(*check);

}

std::vector<const Student *>
System::lookup_family_name(const std::string &family_name) const{
  std::string upper_name = normalize_uppercase(family_name);
  const std::vector<Student*>* check = family_name_index.search(upper_name);

  std::vector<const Student *> blank;
  if(check == nullptr){return blank;}

  return to_const_students(*check);
}

std::vector<const Student *> System::lookup_major(const std::string &major) const{
  const std::vector<Student*>* check = major_index.search(major);

  std::vector<const Student *> blank;
  if(check == nullptr){return blank;}

  return to_const_students(*check);


}

std::vector<const Student *>
System::lookup_full_name(const std::string &given_name, const std::string &family_name) const{
  std::string upper_name = normalize_uppercase(given_name);
  std::string upper_family_name = normalize_uppercase(family_name);
  const std::vector<Student*>* check_name = given_name_index.search(upper_name);
  const std::vector<Student*>* check_family_name = family_name_index.search(upper_family_name);

  std::vector<const Student *> blank;
  if (check_name == nullptr || check_family_name == nullptr) {
    return blank;
  }

  std::vector<Student *> new_vector;
  for(Student* name_ptr : *check_name){
    for(Student* family_ptr : *check_family_name){
      if(name_ptr == family_ptr){
        new_vector.push_back(name_ptr);
        break;
      }
    }
  }

  return to_const_students(new_vector);

}

std::vector<const Student *> System::lookup_all() const{
  return to_const_students(students);

}
