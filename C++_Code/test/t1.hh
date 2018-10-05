#ifndef __STUDENT__HH__
#define __STUDENT__HH__
class Student
{
public:
	Student();
	int getId() const {return id ;}
	int getAge() const;
private:
	int id;
	int age;
};

#endif
