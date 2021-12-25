#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct student {
	char* name;
	long id;
	float grade;
	char hw_grades[5];
	char final_hw_grade;
} student;

typedef struct university {
	student* students;
	int num_of_students;
} university;

int print_menu();
void Option_1(university* uni);
void Option_2(university* uni, FILE* f);
void Option_3(university* uni, FILE* f);
void Option_4(university* uni);
void Option_5(university* uni, FILE* f);
void Option_6(FILE* f);
char HW_Grade(university* uni, int student_index);
void Error_Msg(char* str);
void free_func(student* student, int size);

int main() {
	FILE* f_append;
	int option_value;
	university uni;
	option_value = print_menu();
	while (option_value != 1) {
		printf("First option when entering the program needs to be Option '1'\n\n");
		option_value = print_menu();
	}
	Option_1(&uni);

	if ((f_append = fopen("output.txt", "a")) == NULL) // open / creates output.txt file to append data
		Error_Msg("The input file is wrong");

	while (1) {
		option_value = print_menu();
		switch (option_value) {
		case 1:
			printf("Option 1 can only be selected when entering the program and only 1 time.\n\n");
			break;
		case 2:
			Option_2(&uni,f_append);
			break;
		case 3:
			Option_3(&uni, f_append);
			break;
		case 4:
			Option_4(&uni);
			break;
		case 5:
			Option_5(&uni, f_append);
			break;
		case 6:
			Option_6(f_append);
			fclose(f_append); // close f_append file
			free_func(uni.students, uni.num_of_students); // free students allocations
			return 0;
		default:
			printf("input number invalid (2-6), try again.\n\n");
		}
	}
}

int print_menu() {
	/* The function prints system menu and get input of selected option */
	int option;
	printf("UNIVERSITY SYSTEM MENU\n");
	printf("(1) Students input\n(2) Students output\n(3) Calculate final grade\n(4) New student\n(5) Add bonus\n(6) End of program\nEnter option number: ");
	scanf("%d", &option);
	printf("\n");
	return option;
}

void Option_1(university* uni) {
	/* The function inputs students from input.txt into uni students array*/
	FILE* f;
	student* students = (student*)malloc(sizeof(student)), * temp_arr = NULL; // allocation for students
	int size = 0;
	char name[99];
	if ((f = fopen("input.txt", "r")) == NULL) // open input file to read
		Error_Msg("The input file is wrong");
	while (fscanf(f, "%s %ld %f %s", name, &students[size].id, &students[size].grade, students[size].hw_grades) != EOF) { // input data from file until End Of File
		students[size].name = (char*)malloc(sizeof(char) * (strlen(name) + 1)); // allocation for student name
		if (students[size].name == NULL) {
			printf("No enought memory");
			free_func(students, size);
			exit(1);
		}
		strcpy(students[size].name, name);
		size++;
		temp_arr = (student*)realloc(students, (size + 1) * sizeof(student)); // re-allocation for new students added
		if (temp_arr != NULL) {
			students = temp_arr;
		}
		else {
			free_func(students, size);
			Error_Msg("allocation problem");
		}
	}
	uni->num_of_students = size; // update number of students
	uni->students = students; // sets uni students to the array created
	fclose(f);
	printf("Option '1' Done successfully\n\n");
}

void Option_2(university* uni, FILE* f) {
	/* The function creates output.txt file and print into it all students information from uni.students array*/
	int i;
	fprintf(f, "\nOption 2:\n");
	for (i = 0; i < uni->num_of_students; i++) {
		uni->students[i].final_hw_grade = HW_Grade(uni, i);
		fprintf(f, "Student %d: %s %ld %.2f %c\n", i + 1, uni->students[i].name, uni->students[i].id, uni->students[i].grade, uni->students[i].final_hw_grade); // prints the students data into the file
	}
	fprintf(f, "\n");
	printf("Option '2' Done successfully\n\n");
}

void Option_3(university* uni, FILE* f) {
	/* The function calculates the final grade for each student and prints into output.txt the grades before and after the calculation by the given format */
	int i;
	float final_grade;
	fprintf(f, "Option 3:\nBEFORE:\n");
	for (i = 0; i < uni->num_of_students; i++) {
		fprintf(f, "Student %d: %s %ld %.2f %c\n", i + 1, uni->students[i].name, uni->students[i].id, uni->students[i].grade, uni->students[i].final_hw_grade); // prints the data BEFORE like asked into output.txt
	}
	fprintf(f, "\nAFTER:\n");
	for (i = 0; i < uni->num_of_students; i++) {
		if (uni->students[i].grade >= 55.0) {
			if (uni->students[i].final_hw_grade == '1') {
				final_grade = 0.85 * uni->students[i].grade + 0.15 * 100.0;
			}
			else {
				final_grade = 0.85 * uni->students[i].grade;
			}
		} else {
			final_grade = uni->students[i].grade;
		}
		fprintf(f, "Student %d: %s %ld %.2f %c final : %.2f\n", i + 1, uni->students[i].name, uni->students[i].id, uni->students[i].grade, uni->students[i].final_hw_grade, final_grade); // prints the data with the final grade into output.txt
	}
	printf("Option '3' Done successfully\n\n");
}

void Option_4(university* uni) {
	/* The function adds new student into the uni.students array from stdin by user */
	int i;
	char name[99];
	student* temp_arr;
	temp_arr = (student*)realloc(uni->students, (uni->num_of_students + 1) * sizeof(student)); // re-allocation for new student
	if (temp_arr != NULL) {
		uni->students = temp_arr;
	} else {
		free_func(uni->num_of_students, uni->num_of_students);
		Error_Msg("allocation problem");
	}
	printf("Option 4:\nEnter student information (name,id,grade,hw[5 grades]: ");
	scanf("%s %ld %f %s", name, &uni->students[uni->num_of_students].id, &uni->students[uni->num_of_students].grade, uni->students[uni->num_of_students].hw_grades);  // input into the students array
	uni->students[uni->num_of_students].name = (char*)malloc(sizeof(char) * (strlen(name) + 1)); // allocation for new student name
	if (uni->students[uni->num_of_students].name == NULL) {
		free_func(uni->students, uni->num_of_students);
		Error_Msg("Not enough memory.");
	}
	strcpy(uni->students[uni->num_of_students].name, name);
	uni->num_of_students++;
	printf("Option '4' Done successfully\n\n");
}

void Option_5(university* uni, FILE* f) {
	/* the function adds bonus to student by given student name input by user, if student found adds the bonus and prints
	all the students data with round number grade, if student isnt found - prints relevant message */
	int i, index_of_student = -1, bonus;
	char name[99];
	printf("Option 5:\nEnter student name and points for bonus increase: ");
	scanf("%s %d", name, &bonus); // input studnet name, bonus.
	fprintf(f, "\nOPTION 5:\n");
	for (i = 0; i < uni->num_of_students; i++) {
		if (strcmp(name, uni->students[i].name) == 0) { // checks if the student in the array == student want to add bonus to
			index_of_student = i;
			uni->students[i].grade += bonus; // updates the student grade
			if (uni->students[i].grade > 100) { // if the grade after bonus is over 100 -> update to 100.
				uni->students[i].grade = 100;
			}
		}
	}
	if (index_of_student != -1) { // if student name found
		for (i = 0; i < uni->num_of_students; i++) {
			fprintf(f, "Student %d: %s %ld %d\n", i + 1, uni->students[i].name, uni->students[i].id, (int)round(uni->students[i].grade));
		}
	} else {
		fprintf(f, "No student with the name %s was found.\n", name);
	}
}

void Option_6(FILE* f) {
	/* The function ends the program, prints at the end of the output.txt file relevant message */
	fprintf(f, "\nEnd Of Program");
	printf("\nEnd Of Program, Exiting menu.");
}

char HW_Grade(university* uni, int student_index) {
	/* The function calculate Homework final grade */
	int i, j, grade_counter = 0;
	for (j = 0; j < 5; j++) {
		if (uni->students[student_index].hw_grades[j] == '1') {
			grade_counter++;
		}
	}
	if (grade_counter >= 3) // if 3+ grades are '1'
		return '1';
	return '0';
}

void Error_Msg(char* str) {
	/* Function to handle errors */
	printf("\n%s", str);
	exit(1);
}

void free_func(student* student, int size) {
	/* the function free student array, student.name allocations */
	int i;
	for (i = 0; i < size; i++) {
		free(student[i].name);
	}
	free(student);
}
