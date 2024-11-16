#include<stdio.h>
#include<string.h>

#define MAX_USERS 1000
#define MAX_EXPENSES 1000
#define MAX_NO_OF_FAMALIES 100
#define MAX_NO_OF_INDIVIDUALS 4
#define MAX_EXPENSE_CATEGORY_SIZE 15
#define SUCCESS 1
#define FAILURE 0

struct dates{
    unsigned int date;
    unsigned int month;
    unsigned int year;
};

typedef struct individuals{
    int user_ID;//ranges from 1 to 1000
    char user_name[100];
    float income;
}individuals;

typedef struct expense{
    int expense_ID;
    int user_ID;
    char expense_category[MAX_EXPENSE_CATEGORY_SIZE]; // rent, utility, grocery, stationary, leisure;
    float expense_amount;
    struct dates date_of_expense;
}expense;

typedef struct family{
    int family_ID;
    char family_name[100];
    int total_members;
    int family_members_user_id[4];
    float total_family_income;
    float total_family_expense;
}family;

struct daily_expense{
    struct dates date;
    float total_expense;
};

struct category_expense{
    char category_name[15];
    float expense;
};

struct family_expense_cat_name{
    int user_id;
    float expense;
};

int AddUser(individuals users[], int users_count, int user_id, float income, char name[],int size) {
    int status_code;
    if (users_count >= size || users_count < 0) {
        printf("User list is out of bounds.\n");
        status_code = FAILURE;
    } else {
        int found = 0;
        for (int i = 0; i < users_count; i++) {
            if (users[i].user_ID == user_id) {
                found = 1;
            }
        }

        if (found == 0) {
            users[users_count].user_ID = user_id; 
            users[users_count].income = income;
            strcpy(users[users_count].user_name, name); 
            users_count++; 
            status_code = SUCCESS;
        } else {
            printf("User with user_id %d already exists.", user_id);
            status_code = FAILURE;
        }
    }

    return status_code;
}
void swap_user(individuals user[], int start, int pivot){
    if(start != pivot){
        individuals temp = user[start];
        user[start] = user[pivot];
        user[pivot] = temp;
    }
}

int partition_array_of_user(individuals user[], int start, int end){
    int x = user[start].user_ID;
    int i = start + 1;
    int j = end;
    while(i <= j){
        while(i <= end && user[i].user_ID <= x){
            i++;
        }
        while(j >= start && user[j].user_ID > x){
            j--;
        }
        if(i < j){
            swap_user(user, i, j);
        }
    }
    swap_user(user, start, j);
    return j;
}

void sort_users_using_user_id(individuals user[], int start, int end){
    if(start < end){
        int pivot = partition_array_of_user(user, start, end);
        sort_users_using_user_id(user, start, pivot-1);
        sort_users_using_user_id(user, pivot+1, end);
    }
}

int AddExpense(expense expenses[], int expense_count, individuals users[], int size, int expense_id, int user_id, float expense_amount, char expenses_category[], struct dates date){
    int status_code = SUCCESS;
    if(expense_count >= MAX_EXPENSES){
        status_code = FAILURE;
    }else{
        if(user_id < 1 || user_id >1000){
            printf("invalid user_id\n");
            status_code = FAILURE;
        }else{
            int found = 0;
            for (int i = 0; i < size; i++) {
                if (users[i].user_ID == user_id) {
                    found = 1;
                }
            }
            if(found == 0){
                printf("there don't exists a user with such user_id\n");
                status_code = FAILURE;
            }else{
                int is_there = 0;
                for(int j = 0; j < expense_count && is_there == 0; j++){
                    if(expenses[j].expense_ID == expense_id){
                        is_there = 1;
                    }
                }
                if(is_there == 0){
                    expenses[expense_count].expense_ID = expense_id;
                    expenses[expense_count].expense_amount = expense_amount;
                    expenses[expense_count].user_ID = user_id;
                    strcpy(expenses[expense_count].expense_category, expenses_category);
                    expenses[expense_count].date_of_expense = date;
                    expense_count = expense_count + 1;
                }else{
                    printf("expense id exists\n");
                    status_code = FAILURE;
                }
            }
        }
    }
    return status_code;
}

void swap_expenses(expense expenses[], int i, int j){
    expense temp_expense = expenses[i];
    expenses[i] = expenses[j];
    expenses[j] = temp_expense;
}

int partition_array_of_expenses(expense expenses[], int start, int end){
    int x = expenses[start].user_ID;
    int i = start + 1;
    int j = end;
    while(i <= j){
        while(i <= end && expenses[i].user_ID <= x){
            i++;
        }
        while(j >= start && expenses[j].user_ID > x){
            j--;
        }
        if(i < j){
            swap_expenses(expenses, i, j);
        }
    }
    swap_expenses(expenses, start, j);
    return j;
}

void sort_expenses_by_user_id(expense expenses[], int start, int end){
    if(start < end){
        int pivot = partition_array_of_expenses(expenses, start, end);
        sort_expenses_by_user_id(expenses, start, pivot-1);
        sort_expenses_by_user_id(expenses, pivot+1, end);
    }
}

void sort_expenses_id_of_expenses(expense expenses[], int start, int end){
    for(int i = start+1; i <= end; i++){
        expense x = expenses[i];
        int j = i-1;
        while(j >= start && expenses[j].expense_ID > x.expense_ID){
            expenses[j+1] = expenses[j];
            j= j -1;
        }
        expenses[j+1] = x; 
    }
}

void sort_expenses(expense expenses[], int size){
    sort_expenses_by_user_id(expenses, 0, size-1);
    int i = 0;
    while(i < size){
        int x = expenses[i].user_ID;
        int j = i;
        while(j < size && expenses[j].user_ID == x){
            j++;
        }
        sort_expenses_id_of_expenses(expenses, i, j-1);
        i = j;
    }
}

int CreateFamily(individuals user[], int user_count, family families[], int size, char family_name[], int family_id, int family_user_id[], int total_members) {
    int status_code = SUCCESS;
    if (size > MAX_NO_OF_FAMALIES) {  
        printf("Out of limit\n");
        status_code = FAILURE;
    } else {
        if (total_members > 4 || total_members < 1) {
            status_code = FAILURE;
            printf("A family should contain minimum 1 and maximum 4 members only\n");
        } else {
            int count = 0;
            for (int i = 0; i < total_members; i++) {
                for (int j = 0; j < user_count; j++) {
                    if (user[j].user_ID == family_user_id[i]) { 
                        count++;
                    }
                }
            }
            if (count == total_members) {
                int found = 0, i = 0;
                while (found == 0 && i < size) {
                    if (families[i].family_ID == family_id) {
                        found = 1;
                    } else {
                        i++;
                    }
                }
                if (found == 1) {
                    printf("Family ID already exists\n");
                    status_code = FAILURE;
                } else {
                    families[size].family_ID = family_id;
                    families[size].total_members = total_members;
                    strcpy(families[size].family_name, family_name);
                    for (int i = 0; i < total_members; i++) {
                        families[size].family_members_user_id[i] = family_user_id[i];
                    }
                }
            }
        }
    }

    return status_code;
}

float calculate_monthly_expense_of_family(family fam, int total_members, expense expenses[], int expense_count){
    float sum = 0;
    for(int i = 0; i < total_members; i++){
        for(int j = 0; j < expense_count; j++){
            if(expenses[j].user_ID == fam.family_members_user_id[i]){
                sum += expenses[j].expense_amount;
            }
        }
    }
    return sum;
}

float calculate_monthly_income_of_family(family fam, int total_members, individuals user[], int current_user){
    float sum = 0;
    for(int i = 0; i < total_members; i++){
        for(int j = 0; j < current_user; j++){
            if(user[j].user_ID == fam.family_members_user_id[i]){
                sum += user[j].income;
            }
        }
    }
    return sum;
}

int update_individual_details(individuals user[], int *current_users, family families[], int current_family_count, int user_id, float income, char name[]){
    int found = -1;
    int status_code = SUCCESS;
    int i = 0;
    for(; i < *current_users && found == -1; i++){
        if(user[i].user_ID == user_id){
            found = i;
        }
    }
    if((found != -1) && (found < *current_users)){
        float temp_income = user[found].income;
        user[found].income = income;
        strcpy(user[found].user_name, name);
        for(int i = 0; i < current_family_count; i++){
            for(int j = 0; j < families[i].total_members; j++){
                if(families[i].family_members_user_id[j] == user_id){
                    families[i].total_family_income += (income - temp_income);
                }
            }
        }
    }else{
        int is_free_loc = -1;
        for(int i = 0; i < MAX_USERS && is_free_loc == -1; i++){
            if((user[i].income == -1) && (user[i].user_ID == -1) && (user[i].user_name[0] == '\0')){
                is_free_loc = i;
            }
        }
        if(is_free_loc < MAX_USERS){
            user[is_free_loc].user_ID = user_id;
            user[is_free_loc].income = income;
            strcpy(user[is_free_loc].user_name, name);
            (*current_users) += 1;
        }else {
            status_code = FAILURE;
        }
    }
    return status_code;
}

int delete_individual_details(individuals user[], int *current_users, family families[], int *current_family_count, expense expenses[], int *current_expenses, int user_id){
    int found = -1;
    int status_code = SUCCESS;
    int i = 0;
    for(; i < *current_users && found == -1; i++){
        if(user[i].user_ID == user_id){
            found = i;
        }   
    }
    if((found != -1) && (found < *current_users)){
        float temp_income = user[found].income;
        user[found].user_ID = -1;
        user[found].income = -1;
        user[found].user_name[0] = '\0';
        for(int i = 0; i < *current_family_count; i++){
            int is_done = 0;
            for(int j = 0; j < families[i].total_members && is_done == 0; j++){
                if(families[i].family_members_user_id[j] == user_id){
                    families[i].total_family_income -= temp_income;
                    families[i].family_members_user_id[j] = -1;
                }
                for (int k = j; k < families[i].total_members - 1; k++) {
                    families[i].family_members_user_id[k] = families[i].family_members_user_id[k + 1];
                }
                families[i].total_members -= 1;
                families[i].family_members_user_id[families[i].total_members] = -1;
                is_done = 1;
                if(families[i].total_members == 0){
                    families[i].family_ID = -1;
                    families[i].family_name[0] = '\0';
                    families[i].total_members = 0;
                    families[i].total_family_income = 0;
                    families[i].total_family_expense = 0;
                    families[i].family_members_user_id[families[i].total_members] = -1;
                    for(int l = i; l < *current_family_count - 1; l++){
                        families[l].family_ID = families[l+1].family_ID;
                        strcpy(families[l].family_name, families[l+1].family_name);
                        families[l].total_members = families[l+1].total_members;
                        families[l].total_family_income = families[l+1].total_family_income;
                        families[l].total_family_expense = families[l+1].total_family_expense;
                        for(int m = 0; m < MAX_NO_OF_INDIVIDUALS; m++){
                            families[l].family_members_user_id[m] = families[l+1].family_members_user_id[m];
                        }
                    }
                    families[*current_family_count - 1].family_ID = -1;
                    families[*current_family_count - 1].family_name[0] = '\0';
                    families[*current_family_count - 1].total_members = 0;
                    families[*current_family_count - 1].total_family_income = 0;
                    families[*current_family_count - 1].total_family_expense = 0;
                    for (int j = 0; j < MAX_NO_OF_INDIVIDUALS; j++) {
                        families[*current_family_count - 1].family_members_user_id[j] = -1;
                    }
                    *(current_family_count) -= 1;
                } 
            }
        }
        for(int i = 0; i < *current_expenses; i++){
            if(expenses[i].user_ID == user_id){
                expenses[i].user_ID = -1;
                expenses[i].expense_ID = -1;
                expenses[i].expense_category[0] = '\0';
                expenses[i].expense_amount = 0;
                expenses[i].date_of_expense.date = 0;
                expenses[i].date_of_expense.month = 0;
                expenses[i].date_of_expense.year = 0;
                for (int j = i; j < *current_expenses - 1; j++) {
                    expenses[j].user_ID = expenses[j + 1].user_ID;
                    expenses[j].expense_ID = expenses[j+1].expense_ID;
                    strcpy(expenses[j].expense_category, expenses[j+1].expense_category);
                    expenses[j].expense_amount = expenses[j+1].expense_amount;
                    expenses[j].date_of_expense.date = expenses[j+1].date_of_expense.date; 
                    expenses[j].date_of_expense.month = expenses[j+1].date_of_expense.month;
                    expenses[j].date_of_expense.year = expenses[j+1].date_of_expense.year;
                }
                (*current_expenses)--;
                expenses[*current_expenses].user_ID = -1;
                expenses[*current_expenses].expense_ID = -1;
                expenses[*current_expenses].expense_category[0] = '\0';
                expenses[*current_expenses].expense_amount = 0;
                expenses[*current_expenses].date_of_expense.date = 0;
                expenses[*current_expenses].date_of_expense.month = 0;
                expenses[*current_expenses].date_of_expense.year = 0;
                i--; 
            }
        }
        for (int i = found; i < *current_users - 1; i++) {
            user[i].income = user[i + 1].income;
            user[i].user_ID = user[i + 1].user_ID;
            strcpy(user[i].user_name, user[i+1].user_name); 
        }
        user[*current_users - 1].user_ID = -1;
        user[*current_users - 1].income = -1;
        user[*current_users - 1].user_name[0] = '\0';
        (*current_users) -= 1;
    }else{
        printf("user not found\n");
        status_code = FAILURE;
    } 
    return status_code;
}

int update_family_details(family families[], int *current_family_count, int family_id, char family_name[], int user_id_arr[], int total_users){
    int found = -1;
    int status_code = SUCCESS;
    for(int i = 0; i < *current_family_count && found == -1; i++){
        if(families[i].family_ID == family_id){
            found = i;
        }
    }
    if(found != -1){
        strcpy(families[found].family_name, family_name);
        families[found].total_members = total_users;
        for(int i = 0; i < total_users; i++){
            families[found].family_members_user_id[i] = user_id_arr[i];
        }
    }else{
        int is_free_loc = 0;
        for(int i = 0; i < MAX_NO_OF_FAMALIES && is_free_loc == 0; i++){
            if(families[i].family_ID == -1){
                families[i].family_ID = family_id;
                families[i].total_members = total_users;
                strcpy(families[i].family_name, family_name);
                for(int j = 0; j < total_users; j++){
                    families[i].family_members_user_id[j] = user_id_arr[j]; 
                }
                is_free_loc = 1;
            }
        }
        if(is_free_loc == 1){
            (*current_family_count) += 1;
        }else{
            status_code = FAILURE; 
        }
    }
    return status_code;
}

int delete_family(family families[], int *current_family_count, individuals user[], int *current_users, expense expenses[], int *current_expense, int fam_id){
    int found = -1;
    int status_code = SUCCESS;
    for(int i = 0; i < *current_family_count; i++){
        if(families[i].family_ID == fam_id){
            found = i;
        }
    }
    if(found != -1){
        int user_id_arr[MAX_NO_OF_INDIVIDUALS];
        int total_family_members = families[found].total_members;
        for(int i = 0; i < total_family_members; i++) {
            user_id_arr[i] = families[found].family_members_user_id[i];
        }
        for(int i = 0; i < total_family_members; i++) {
            int user_id = user_id_arr[i];
            int is_done = 0;
            for(int j = 0; j < *current_users && is_done == 0; j++) {
                if(user[j].user_ID == user_id) {
                    for(int k = j; k < *current_users - 1; k++) {
                        user[k].income = user[k + 1].income;
                        user[k].user_ID = user[k + 1].user_ID;
                        strcpy(user[k].user_name, user[k + 1].user_name); 
                    }
                    (*current_users)--;
                    is_done = 1;
                }
            }
        }
        for(int i = 0; i < total_family_members; i++) {
            int user_id = user_id_arr[i];
            for(int j = 0; j < *current_expense; j++) {
                if(expenses[j].user_ID == user_id) {
                    for(int k = j; k < *current_expense - 1; k++) {
                        expenses[k].user_ID = expenses[k+1].user_ID;
                        expenses[k].expense_ID = expenses[k+1].expense_ID;
                        strcpy(expenses[k].expense_category, expenses[k+1].expense_category);
                        expenses[k].expense_amount = expenses[k+1].expense_amount;
                        expenses[k].date_of_expense.date = expenses[k+1].date_of_expense.date; 
                        expenses[k].date_of_expense.month = expenses[k+1].date_of_expense.month;
                        expenses[k].date_of_expense.year = expenses[k+1].date_of_expense.year;
                    }
                    (*current_expense)--;
                    j--; 
                }
            }
        }
        float family_income = families[found].total_family_income;
        float family_expenses = families[found].total_family_expense;
        for(int i = found; i < *current_family_count - 1; i++) {
            families[i].family_ID = families[i+1].family_ID;
            strcpy(families[i].family_name, families[i+1].family_name);
            families[i].total_members = families[i+1].total_members;
            families[i].total_family_income = families[i+1].total_family_income;
            families[i].total_family_expense = families[i+1].total_family_expense;
            for(int m = 0; m < MAX_NO_OF_INDIVIDUALS; m++){
                families[i].family_members_user_id[m] = families[i+1].family_members_user_id[m];
            }
        }
        (*current_family_count)--;
    }else{
        status_code = FAILURE;
    }   
    return status_code;
}

int update_expense_details(expense expenses[], int *current_expenses, int user_id, int expense_id, float expense_amount, char name[], struct dates date){
    int status_code = SUCCESS;
    int found = -1;
    for(int i = 0; i < *current_expenses && found == -1; i++){
        if(expenses[i].user_ID == user_id && expenses[i].expense_ID == expense_id){
            found = i;
        }
    }
    if(found != -1){
        expenses[found].expense_amount = expense_amount;  
        strcpy(expenses[found].expense_category, name);  
        expenses[found].date_of_expense = date;           
    }else{
        int is_free_loc = 0;
        for(int i = 0; i < MAX_EXPENSES && is_free_loc == 0; i++) {
            if(expenses[i].expense_ID == -1) {  
                expenses[i].expense_ID = expense_id;
                expenses[i].user_ID = user_id;                   
                expenses[i].expense_amount = expense_amount;     
                strcpy(expenses[i].expense_category, name);      
                expenses[i].date_of_expense = date;               
                is_free_loc = 1;
            }
        }
        if(is_free_loc == 1) {
            (*current_expenses) += 1;
        }
        else{
            status_code = FAILURE;
        }
    }
    return status_code;
}

int delete_expense(expense expenses[], int *current_expenses, individuals user[], int *current_users, family families[], int *current_family_count, int expense_id){
    int found = -1;
    int status_code = SUCCESS;
    for(int i = 0; i < *current_expenses && found == -1; i++) {
        if(expenses[i].expense_ID == expense_id) {
            found = i;
        }
    }
    if(found != -1) {
        int user_id = expenses[found].user_ID;
        float expense_amount = expenses[found].expense_amount;
        for(int i = 0; i < *current_family_count; i++) {
            int is_done = 0;
            for(int j = 0; j < families[i].total_members && is_done == 0; j++) {
                if(families[i].family_members_user_id[j] == user_id) {
                    families[i].total_family_expense -= expense_amount; 
                    is_done = 1;
                }
            }
        }
        for(int i = found; i < *current_expenses - 1; i++) {
            expenses[i].user_ID = expenses[i+1].user_ID;
            expenses[i].expense_ID = expenses[i+1].expense_ID;
            strcpy(expenses[i].expense_category, expenses[i+1].expense_category);
            expenses[i].expense_amount = expenses[i+1].expense_amount;
            expenses[i].date_of_expense.date = expenses[i+1].date_of_expense.date; 
            expenses[i].date_of_expense.month = expenses[i+1].date_of_expense.month;
            expenses[i].date_of_expense.year = expenses[i+1].date_of_expense.year;
        }
        (*current_expenses)--;
        expenses[*current_expenses].expense_ID = -1;
        expenses[*current_expenses].user_ID = -1;
        expenses[*current_expenses].expense_category[0] = '\0';
        expenses[*current_expenses].expense_amount = 0;
        expenses[*current_expenses].date_of_expense.date = 0;
        expenses[*current_expenses].date_of_expense.month = 0;
        expenses[*current_expenses].date_of_expense.year = 0;
    } else {
        status_code = FAILURE;
    }
    return status_code;
}

float get_total_expense(family fam){
    float monthly_expense = fam.total_family_expense;
    if(monthly_expense > fam.total_family_income){
        printf("you have surpassed you expense limit by %f\n", monthly_expense - fam.total_family_income);
    }
    return monthly_expense;
}

struct daily_expense max_expense_day_of_family(family fam, int total_members, expense expenses[], int current_expense){//family maxexpense day
    struct daily_expense expense_dates[MAX_EXPENSES];
    for(int i = 0; i < MAX_EXPENSES; i++){
        expense_dates[i].date.date = 0;
        expense_dates[i].date.month = 0;
        expense_dates[i].date.year = 0;
        expense_dates[i].total_expense = 0;
    }
    int daily_count = 0;
    for(int i = 0; i < total_members; i++){
        for(int j = 0; j < current_expense; j++){
            if(fam.family_members_user_id[i] == expenses[j].user_ID){
                int found = 0;
                for(int k = 0; (k < daily_count) && (found == 0); k++){
                    if((expense_dates[k].date.date == expenses[j].date_of_expense.date) && (expense_dates[k].date.month == expenses[j].date_of_expense.month) && (expense_dates[k].date.year == expenses[j].date_of_expense.year)){
                        expense_dates[k].total_expense += expenses[j].expense_amount;
                        found = 1;
                    }
                }
                if(found == 0){
                    if(daily_count < MAX_EXPENSES){
                        expense_dates[daily_count].date.date = expenses[j].date_of_expense.date;
                        expense_dates[daily_count].date.month = expenses[j].date_of_expense.month;
                        expense_dates[daily_count].date.year = expenses[j].date_of_expense.year;
                        expense_dates[daily_count].total_expense = expenses[j].expense_amount;
                        daily_count += 1;
                    }
                }
            }
        }
    }
    int max_expense = 0;
    struct daily_expense max_expense_date;
    for(int i = 0; i < daily_count; i++){
        if(max_expense < expense_dates[i].total_expense){
            max_expense = expense_dates[i].total_expense;
            max_expense_date.total_expense = expense_dates[i].total_expense;
            max_expense_date.date.date = expense_dates[i].date.date;
            max_expense_date.date.month = expense_dates[i].date.month;
            max_expense_date.date.year = expense_dates[i].date.year;
        }
    }
    return max_expense_date;
}

float get_individual_expense(individuals person, expense expenses[], int current_expense, struct category_expense category_wise[]){
    float sum = 0;
    for(int i = 0; i < current_expense; i++){
        if(person.user_ID == expenses[i].user_ID){
            sum += expenses[i].expense_amount;
            if(strcmp(expenses[i].expense_category , "rent") == 0){
                category_wise[0].expense += expenses[i].expense_amount;
            }else if(strcmp(expenses[i].expense_category , "utility") == 0){
                category_wise[1].expense += expenses[i].expense_amount;
            }else if(strcmp(expenses[i].expense_category , "grocery") == 0){
                category_wise[2].expense += expenses[i].expense_amount;
            }else if(strcmp(expenses[i].expense_category , "stationary") == 0){
                category_wise[3].expense += expenses[i].expense_amount;
            }else if(strcmp(expenses[i].expense_category , "leisure") == 0){
                category_wise[4].expense += expenses[i].expense_amount;
            }
        }
    }
    return sum;
}

void sort_category_wise(struct category_expense category_wise[], int start, int end){
    for(int i = start+1; i <= end; i++){
        float comp = category_wise[i].expense;
        int j = i - 1;
        char temp_name[15];
        strcpy(temp_name, category_wise[i].category_name);
        while(j >= start && category_wise[j].expense < comp){
            category_wise[j+1].expense = category_wise[j].expense;
            strcpy(category_wise[j+1].category_name, category_wise[j].category_name);
            j = j - 1;
        }
        category_wise[j+1].expense = comp;
        strcpy(category_wise[j+1].category_name, temp_name);
    }
}

float get_categorical_expense(char category_name[], family fam, int current_family_count, expense expenses[], int current_expense, struct family_expense_cat_name fecn[]){
    float total_expense = 0;
    int size = fam.total_members;
    int fecn_index = 0;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < current_expense; j++){
            if(fam.family_members_user_id[i] == expenses[j].user_ID){
                if(strcmp(expenses[j].expense_category, category_name) == 0){
                    total_expense += expenses[j].expense_amount;
                    int exists = 0;
                    for(int k = 0; k < fecn_index && exists == 0; k++){
                        if(fecn[k].user_id == expenses[j].user_ID){
                            fecn[k].expense += expenses[j].expense_amount;
                            exists = 1;
                        }
                    }
                    if(!exists && fecn_index < size){
                        fecn[fecn_index].expense = expenses[j].expense_amount;
                        fecn[fecn_index].user_id = expenses[j].user_ID;
                        fecn_index++;
                    }
                }
            }
        }
    }
    return total_expense;
}

void sort_fecn(struct family_expense_cat_name fecn[], int start, int end){
    for(int i = start+1; i <= end; i++){
        float x = fecn[i].expense;
        int temp = fecn[i].user_id;
        int j = i - 1;
        while(j >= start && fecn[j].expense < x){
            fecn[j+1].expense = fecn[j].expense;
            fecn[j+1].user_id = fecn[j].user_id;
            j = j - 1;
        }
        fecn[j+1].expense = x;
        fecn[j+1].user_id = temp;
    }
}   

void initialize_user(individuals user[], int size){
    for(int i = 0; i < size; i++){
        user[i].income = -1;
        user[i].user_ID = -1;
        user[i].user_name[0] = '\0';
    }
}

void initialize_family(family families[], int size) {
    for (int i = 0; i < size; i++) {
        families[i].family_ID = -1;
        families[i].family_name[0] = '\0';
        families[i].total_members = 0;
        families[i].total_family_income = 0;
        families[i].total_family_expense = 0;
        for (int j = 0; j < MAX_NO_OF_INDIVIDUALS; j++) {
            families[i].family_members_user_id[j] = -1;
        }
    }
}

void initialize_expenses(expense expenses[], int size){
    for(int i = 0; i < size; i++){
        expenses[i].user_ID = -1;
        expenses[i].expense_ID = -1;
        expenses[i].expense_category[0] = '\0';
        expenses[i].expense_amount = 0;
        expenses[i].date_of_expense.date = 0;
        expenses[i].date_of_expense.month = 0;
        expenses[i].date_of_expense.year = 0;
    }
}

void read_user(int *current_users, individuals user[], int input_users){
    FILE *fp = fopen("file1.txt", "r");
    if (fp == NULL) {
        printf("Cannot open file1.txt, the program will be stopped now\n");
        return;
    }

    int flag = 0;

    while (*current_users < input_users && flag == 0) {
        int result = fscanf(fp, "%d %49s %f", 
                            &user[*current_users].user_ID, 
                            user[*current_users].user_name, 
                            &user[*current_users].income);
        if (result == 3) {
            (*current_users) += 1;
        } else if (result == EOF) {
            flag = 1;
        } else {
            printf("Error reading data at user %d\n", *current_users);
            flag = 1;
        }
    }
    fclose(fp);
    printf("Data read from file.txt\n");
}

void read_expenses(int *current_expenses, expense expenses[], int input_expenses){
    FILE *fp2 = fopen("file2.txt", "r");
    if (fp2 == NULL) {
        printf("Cannot open file2.txt, the program will be stopped now\n");
        return;
    }

    int flag = 0;

    while (*current_expenses < input_expenses && flag == 0) {
        int result = fscanf(fp2, "%d %d %f %49s %u %u %u", 
                            &expenses[*current_expenses].user_ID, 
                            &expenses[*current_expenses].expense_ID, 
                            &expenses[*current_expenses].expense_amount, 
                            expenses[*current_expenses].expense_category, 
                            &expenses[*current_expenses].date_of_expense.date, 
                            &expenses[*current_expenses].date_of_expense.month, 
                            &expenses[*current_expenses].date_of_expense.year);
        if (result == 7) {
            *current_expenses += 1;
        } else if (result == EOF) {
            flag = 1; 
        } else {
            printf("Error reading data at expense %d\n", *current_expenses);
            flag = 1; 
        }
    }
    fclose(fp2);
    printf("Data read from file2.txt\n");
}

void read_families(int *current_family_count, family families[], int total_families){
    FILE *fp3 = fopen("file3.txt", "r");
    if (fp3 == NULL) {
        printf("Cannot open file3.txt, the program will be stopped now\n");
        return;
    }

    int flag = 0;

    while (*current_family_count < total_families && flag == 0) {
        int result = fscanf(fp3, "%d %49s %d %d %d %d %d", 
                            &families[*current_family_count].family_ID, 
                            families[*current_family_count].family_name, 
                            &families[*current_family_count].total_members, 
                            &families[*current_family_count].family_members_user_id[0], 
                            &families[*current_family_count].family_members_user_id[1], 
                            &families[*current_family_count].family_members_user_id[2], 
                            &families[*current_family_count].family_members_user_id[3]);
        if (result == 7) {
            *current_family_count += 1;
        } else if (result == EOF) {
            flag = 1; 
        } else {
            printf("Error reading data at family %d\n", *current_family_count);
            flag = 1; 
        }
    }
    fclose(fp3);
    printf("Data read from file3.txt\n");
}

int main(){
    int input_users, input_expenses, total_families;

    do{
        printf("enter total number of users : ");
        scanf("%d", &input_users);
    }while(input_users < 25);

    do{
        printf("enter total number of expenses : ");
        scanf("%d", &input_expenses);
    }while(input_expenses < 40);

    do{
        printf("enter total number of families : ");
        scanf("%d", &total_families);
    }while(total_families < 10);

    individuals user[MAX_USERS];
    expense expenses[MAX_EXPENSES];
    family families[MAX_NO_OF_FAMALIES];

    initialize_user(user, MAX_USERS);
    initialize_expenses(expenses, MAX_EXPENSES);
    initialize_family(families, MAX_NO_OF_FAMALIES);

    int current_users = 0;
    int current_expenses = 0;
    int current_family_count = 0;
    
    read_user(&current_users, user, input_users);
    read_expenses(&current_expenses, expenses, input_expenses);
    read_families(&current_family_count, families, total_families);

    sort_users_using_user_id(user, 0, current_users-1);
    sort_expenses(expenses, current_expenses);

    for(int i = 0; i < current_family_count; i++){
        families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
        families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
    }

    printf("user : %d, expenses : %d, family : %d\n", current_users, current_expenses, current_family_count);

    int n = 1;
    while(n < 11 && n != 10 && n > 0){
        printf("Enter 1  : Add User\n");
        printf("Enter 2  : Add Expense\n");
        printf("Enter 3  : Create Family\n");
        printf("Enter 4  : Update or Delete individual family\n");
        printf("Enter 5  : Update or Delete Expense\n");
        printf("Enter 6  : Get Total Expense\n");
        printf("Enter 7  : Get Categorical Expense\n");
        printf("Enter 8  : Get Highest Expense Day\n");
        printf("Enter 9  : Get Individual Expense\n");
        printf("Enter 10 : Exit\n");
        printf("enter n : ");
        scanf("%d", &n);

        switch(n){
            case 1 :{
                printf("enter the details\n");
                int user_id;
                float income;
                char name[100];
                printf("enter user_id : ");
                scanf("%d",&user_id);
                printf("enter income of user : ");
                scanf("%f", &income);
                printf("enter name of individual user : ");
                scanf(" %s", name);
                while(user_id > 1000 || user_id < 1){
                    printf("wrong input\n");
                    printf("enter user_id : ");
                    scanf("%d",&user_id);
                }
                while(income < 0){
                    printf("wrong input\n");
                    printf("enter income of user : ");
                    scanf("%f", &income);
                }
                while(name[0] == '\0'){
                    printf("wrong input\n");
                    printf("enter name of individual user : ");
                    fgets(name, 100, stdin);
                }
                int status = AddUser(user, current_users, user_id, income, name, MAX_USERS);
                if(status == SUCCESS){
                    printf("user added succesfully\n");
                    current_users = current_users + 1;
                    sort_users_using_user_id(user, 0, current_users-1);
                    for(int i=0;i<current_users;i++){
                        printf("%d %f %s \n",user[i].user_ID,user[i].income,user[i].user_name);
                    }
                }else{
                    printf("you got following error\n");
                }

                break;
            }
            case 2 :{
                int user_id, expense_id;
                float expense_amount;
                char expense_category[MAX_EXPENSE_CATEGORY_SIZE];
                struct dates date;
                printf("Enter user ID for expense: ");
                scanf("%d", &user_id);
                printf("Enter expense ID for expense: ");
                scanf("%d", &expense_id);
                printf("Enter expense amount: ");
                scanf("%f", &expense_amount);
                printf("Enter expense category: ");
                scanf("%s", &expense_category);
                printf("Enter date of expense (DD MM YYYY): ");
                scanf("%u %u %u", &date.date, &date.month, &date.year);
                while(expense_amount < 0){
                    printf("enter income again : ");
                    scanf("%f", &expense_amount);
                }
                while(strcmp(expense_category, "rent") != 0 && strcmp(expense_category, "utility") != 0 && strcmp(expense_category, "stationary") != 0 && strcmp(expense_category, "grocery") != 0 && strcmp(expense_category, "leisure") != 0){
                    printf("enter only rent or utility or stationary or grocery or leisure\n");
                    printf("Enter expense category again : ");
                    scanf("%s", &expense_category);         
                }
                int status = AddExpense(expenses, current_expenses, user, current_users, expense_id, user_id, expense_amount, expense_category, date);
                if (status == SUCCESS) {
                    printf("Expense added successfully\n");
                    current_expenses += 1;
                    sort_expenses(expenses, current_expenses);
                    for(int i = 0 ; i < current_expenses; i++){
                        printf("%d %d %f %s\n", expenses[i].user_ID, expenses[i].expense_ID, expenses[i].expense_amount, expenses[i].expense_category);
                    }
                } else {
                    printf("Error adding expense\n");
                }
                break;
             }  
            case 3 :{
                int family_id;
                int total_members; 
                printf("Enter family id: ");
                scanf("%d", &family_id);
                printf("Enter total members in the family: ");
                scanf("%d", &total_members);
                int family_user_id[total_members];  
                for (int i = 0; i < total_members; i++) {
                    printf("Enter user ID of family member %d: ", i + 1);
                    scanf("%d", &family_user_id[i]);
                }
                char family_name[25]; 
                printf("Enter family name: ");
                scanf("%s", family_name);
                int found = 0;
                for(int i = 0; i < current_family_count && found == 0; i++){
                    if(families[i].family_ID == family_id){
                        found = 1;
                    }
                }

                if(found == 0){
                    int exists = 1, id = -1;
                    for(int i = 0; i < total_members && exists == 1; i++){
                        int found = 0;
                        id = family_user_id[i];
                        for(int j = 0; j < current_users && found == 0; j++){
                            if(family_user_id[i] == user[j].user_ID){
                                found = 1;
                            }
                        }
                        if(found == 0){
                            exists = 0;
                        }
                    }
                    if(exists == 1){
                        int status = CreateFamily(user, current_users, families, current_family_count, family_name, family_id, family_user_id, total_members);
                        if (status == SUCCESS) {
                            printf("Family created successfully\n");
                            families[current_family_count].total_family_expense = calculate_monthly_expense_of_family(families[current_family_count], total_members, expenses, current_expenses);
                            families[current_family_count].total_family_income = calculate_monthly_income_of_family(families[current_family_count], total_members, user, current_users);
                            current_family_count++;
                            for (int i = 0; i < current_family_count; i++) {
                                printf("%s %d %d %f %f\n", families[i].family_name, families[i].family_ID, families[i].total_members, families[i].total_family_expense, families[i].total_family_income);
                            }
                        } else {
                            printf("Error creating family\n");
                        }
                    }else{
                        printf("user id %d dont exists\n", id);
                    }
                }else{
                    printf("family already exists\n");
                }
                break;
            }
            case 4:{
                int j;
                printf("enter 1 : update_user\n");
                printf("enter 2 : update_family\n");
                printf("enter 3 : delete user\n");
                printf("enter 4 : delete family\n");
                printf("enter j : ");
                scanf("%d", &j);
                switch(j){
                    case 1:
                    {
                        int user_id;
                        float income;
                        char name[100];
                        printf("enter user id : ");
                        scanf("%d",&user_id);
                        printf("enter current income : ");
                        scanf("%f", &income);
                        printf("enter name : ");
                        scanf("%s", name);
                        int found = 0;
                        for(int i = 0; i < current_users && found == 0; i++){
                            if(user[i].user_ID == user_id){
                                found = 1;
                            }
                        }
                        if(found == 1){
                            int status = update_individual_details(user, &current_users, families, current_family_count, user_id, income, name);
                            if(status == SUCCESS){
                                for(int i=0;i<current_users;i++){
                                    printf("%d %f %s \n",user[i].user_ID,user[i].income,user[i].user_name);
                                }
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("user not found\n");
                        }
                        break;
                    }
                    case 2:{
                        int fam_id;
                        printf("enter fam id : ");
                        scanf("%d", &fam_id);
                        int total_users;
                        printf("enter total members : ");
                        scanf("%d", &total_users);
                        int user_id_arr[4];
                        for(int i = 0; i < total_users; i++){
                            printf("enter user id of user %d : ",i+1);
                            scanf("%d", &user_id_arr[i]);
                        }
                        char family_name[100];
                        printf("enter family name : ");
                        scanf("%s", family_name);
                        int found = 0;
                        for(int i = 0; i < current_family_count && found == 0; i++){
                            if(families[i].family_ID == fam_id){
                                found = 1;
                            }
                        }
                        if(found != 0){
                            int status = update_family_details(families, &current_family_count, fam_id, family_name, user_id_arr, total_users);
                            if(status == SUCCESS){
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("family id not found\n");
                        }
                        break;
                    }
                    case 3:{
                        int user_id;
                        printf("enter user id you want to remove : ");
                        scanf("%d",&user_id);
                        int found = 0;
                        for(int i = 0; i < current_users && found == 0; i++){
                            if(user[i].user_ID == user_id){
                                found = 1;
                            }
                        }
                        if(found == 1){
                            int status = delete_individual_details(user, &current_users, families, &current_family_count, expenses, &current_expenses, user_id);
                            if(status == SUCCESS){
                                for(int i=0;i<current_users;i++){
                                    printf("%d %f %s \n",user[i].user_ID,user[i].income,user[i].user_name);
                                }
                                printf("\n");
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                                for (int i = 0; i < current_family_count; i++) {
                                    printf("%s %d %d %f %f\n", families[i].family_name, families[i].family_ID, families[i].total_members, families[i].total_family_expense, families[i].total_family_income);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("user id dont exist\n");
                        }
                        break;
                    }
                    case 4:{
                        int fam_id;
                        printf("enter family id that you want to delete : ");
                        scanf("%d", &fam_id);
                        int found = 0;
                        for(int i = 0; i < current_family_count; i++){
                            if(families[i].family_ID == fam_id){
                                found = 1;
                            }
                        }
                        if(found == 1){
                            int status = delete_family(families, &current_family_count, user, &current_users, expenses, &current_expenses, fam_id);
                            if(status == SUCCESS){
                                 for(int i=0;i<current_users;i++){
                                    printf("%d %f %s \n",user[i].user_ID,user[i].income,user[i].user_name);
                                }
                                printf("\n");
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                                for (int i = 0; i < current_family_count; i++) {
                                    printf("%s %d %d %f %f\n", families[i].family_name, families[i].family_ID, families[i].total_members, families[i].total_family_expense, families[i].total_family_income);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("family id not found\n");
                        }
                        break;
                    }
                    default:{
                        printf("error occurred\n");
                        break;
                    }
                }
                break;
            }
            case 5:{
                int p;
                printf("enter 1 : update expenses\n");
                printf("enter 2 : delete expenses\n");
                printf("enter your number : ");
                scanf("%d", &p);
                switch(p){
                    case 1:
                    {
                        int user_id;
                        int expense_id;
                        printf("enter user id : ");
                        scanf("%d", &user_id);
                        printf("enter expense id : ");
                        scanf("%d", &expense_id);
                        char name[100];
                        printf("enter category name : ");
                        scanf("%s", &name);
                        struct dates date;
                        printf("enter date : ");
                        scanf("%u %u %u", &date.date, &date.month, &date.year);
                        float expense_amount;
                        printf("enter expense amount : ");
                        scanf("%f", &expense_amount);
                        while(strcmp(name, "rent") != 0 && strcmp(name, "utility") != 0 && strcmp(name, "stationary") != 0 && strcmp(name, "grocery") != 0 && strcmp(name, "leisure") != 0){
                            printf("enter only rent or utility or stationary or grocery or leisure\n");
                            printf("Enter expense category again : ");
                            scanf("%s", &name);         
                        }
                        int found = 0;
                        for(int i = 0; i < current_expenses && found == 0; i++){
                            if(expenses[i].user_ID == user_id && expenses[i].expense_ID == expense_id){
                                found = 1;
                            }
                        }
                        if(found == 1){
                            int status = update_expense_details(expenses, &current_expenses, user_id, expense_id, expense_amount, name, date);
                            if(status == SUCCESS){
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                                for(int i = 0 ; i < current_expenses; i++){
                                    printf("%d %d %f %s\n", expenses[i].user_ID, expenses[i].expense_ID, expenses[i].expense_amount, expenses[i].expense_category);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("expense not found\n");
                        }
                        break;
                    }
                
                    case 2:{
                        int expense_id;
                        printf("enter expense_id use want to remove : ");
                        scanf("%d", &expense_id);
                        int found = 0;
                        for(int i = 0; i < current_expenses && found == 0; i++){
                            if(expenses[i].expense_ID == expense_id){
                                found = 1;
                            }
                        }
                        if(found == 1){
                            int status = delete_expense(expenses, &current_expenses, user, &current_expenses, families, &current_family_count, expense_id);
                            if(status == SUCCESS){
                                for(int i = 0; i < current_family_count; i++){
                                    families[i].total_family_income = calculate_monthly_income_of_family(families[i],families[i].total_members,user, current_users);
                                    families[i].total_family_expense = calculate_monthly_expense_of_family(families[i], families[i].total_members, expenses, current_expenses);
                                }
                                for(int i = 0 ; i < current_expenses; i++){
                                    printf("%d %d %f %s\n", expenses[i].user_ID, expenses[i].expense_ID, expenses[i].expense_amount, expenses[i].expense_category);
                                }
                            }else{
                                printf("process failed\n");
                            }
                        }else{
                            printf("expense not found\n");
                        }
                        break;
                    }
                    default:{
                        printf("some error occurred\n");
                        break;
                    }
                }
                break;
            }
            case 6:
            {
                family fam;
                int fam_id;
                printf("enter family id : ");
                scanf("%d", &fam_id);
                int found = 0;
                for(int i = 0; i < current_family_count && found == 0; i++){
                    if(fam_id == families[i].family_ID){
                        found = 1;
                        fam = families[i];
                    }
                }
                if(found == 1){
                    float ans = get_total_expense(fam);
                    printf("total expenses for the month : %f\n", ans);
                }else{
                    printf("family not found\n");
                }
                break;
            }
            case 7:
            {
                char cat_name[15];
                printf("enter category name : ");
                scanf("%s", &cat_name);
                while(strcmp(cat_name, "rent") != 0 && strcmp(cat_name, "utility") != 0 && strcmp(cat_name, "stationary") != 0 && strcmp(cat_name, "grocery") != 0 && strcmp(cat_name, "leisure") != 0){
                    printf("enter only rent or utility or stationary or grocery or leisure\n");
                    printf("Enter expense category again : ");
                    scanf("%s", &cat_name);         
                }
                family fam;
                int fam_id;
                printf("enter family id : ");
                scanf("%d", &fam_id);
                int found = 0;
                for(int i = 0; i < current_family_count && found == 0; i++){
                    if(fam_id == families[i].family_ID){
                        found = 1;
                        fam = families[i];
                    }
                }
                if(found == 1){
                    int s = fam.total_members;
                    struct family_expense_cat_name
                 fecn[s];
                    for(int i = 0; i < s; i++){
                        fecn[i].expense = 0;
                        fecn[i].user_id = 0;
                    }
                    float total_expense = get_categorical_expense(cat_name, fam, current_family_count, expenses, current_expenses, fecn);
                    printf("total expense of family on %s is %f\n", cat_name, total_expense);
                    sort_fecn(fecn, 0, s-1);
                    for(int i = 0; i < s; i++){
                        printf("user %d : %f\n",fecn[i].user_id, fecn[i].expense);
                    }
                }else{
                    printf("no family found\n");
                }   
                break;
            }
            case 8 : 
            {
                int family_id;
                printf("enter family_id you need to check max expenses : ");
                scanf("%d", &family_id);
                family fam;
                int found = 0;
                for(int i = 0; i < current_family_count && found == 0; i++){
                    if(families[i].family_ID == family_id){
                        fam = families[i];
                        found = 1;
                    }
                }
                if(found == 1){
                    struct daily_expense max_date = max_expense_day_of_family(fam, fam.total_members, expenses, current_expenses);
                    printf("max expense day : ");
                    printf("%d-%d-%d\n", max_date.date.date, max_date.date.month, max_date.date.year);
                    printf("max_expense : %f\n", max_date.total_expense);
                }else{
                    printf("family not found\n");
                }
                break;
            }
            case 9 :
            {
                struct category_expense category_wise[5];
                strcpy(category_wise[0].category_name , "rent");
                strcpy(category_wise[1].category_name , "utility");
                strcpy(category_wise[2].category_name , "grocery");
                strcpy(category_wise[3].category_name , "stationary");
                strcpy(category_wise[4].category_name , "leisure");
                for(int i = 0; i < 5; i++){
                    category_wise[i].expense = 0;
                }
                int user_id;
                printf("enter user id of which users monthly expenses you want to know :  ");
                scanf("%d", &user_id);
                individuals person;
                int found = 0;
                for(int i = 0; i < current_users && found == 0; i++){
                    if(user[i].user_ID == user_id){
                        person = user[i];
                        found = 1;
                    }
                }
                if(found == 1){
                    float total_expense = get_individual_expense(person, expenses, current_expenses, category_wise);
                    sort_category_wise(category_wise, 0, 4);
                    printf("total monthly expense : %f\n",total_expense);
                    for(int i = 0; i < 5; i++){
                        printf("%s : %f\n",category_wise[i].category_name, category_wise[i].expense);
                    }
                }else{
                    printf("user not found\n");    
                }
                break;
            }
            case 10:{
                printf("exited\n");
                break;
            }
            default:{
                printf("error occurred\n");
                break;
            }
        }
    }
   return 0;
}
