#include <stdio.h>


// Lab Marks
 struct lab {
   double experiment1;
   double experiment2;
   double experiment3;
 };

 // Theory Marks
 struct theory {
   double test;
   double final;
 };

 // Course marks and records
 struct ele709_record {
   long ID_number;
   struct lab lab_mark;
   struct theory theory_mark;
 };


double total_mark(struct ele709_record *p) {
  double lab_avg = 0.2 * ((p->lab_mark.experiment1 + p->lab_mark.experiment2 + p->lab_mark.experiment3) / 3);
  double theory_avg = p->theory_mark.test * 0.3 + p->theory_mark.final * 0.5;
  double total_mark = lab_avg + theory_avg;
  return total_mark;
}


int main() {
  // Define final grades

   // Define a sample student
   struct ele709_record john_doe;
   john_doe.ID_number = 12345678;
   john_doe.lab_mark.experiment1 = 90.2;
   john_doe.lab_mark.experiment2 = 70.5;
   john_doe.lab_mark.experiment3 = 80.4;
   john_doe.theory_mark.test = 82.3;
   john_doe.theory_mark.final = 79.2;

   // Define a student using a pointer
   struct ele709_record jane_doe;
   struct ele709_record *p;

   p = &jane_doe;

   p->ID_number = 12345678;
   p->lab_mark.experiment1 = 90.2;
   p->lab_mark.experiment2 = 70.5;
   p->lab_mark.experiment3 = 80.4;
   p->theory_mark.test = 82.3;
   p->theory_mark.final = 79.2;

   printf("John Doe's Final Mark: %lf / 100\n", total_mark(&john_doe));
   printf("Jane Doe's Final Mark: %lf / 100\n", total_mark(p));
   printf("\nNote: The Jane and John Doe marks simply represent pointer vs direct refrence to the assignment of the individual grade values\n");

}
