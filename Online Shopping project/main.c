#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#define TRUE 1
#define FALSE 0

typedef struct {
    char fname[30];
    char lname[30];
}name;

typedef struct {
    int day,month,year;
}dat;

typedef struct {
    char door_no[10];
    char address_line1[100];
    char city[20];
    char state[20];
}address;

typedef struct {
    char login_name[100];
    char password[100];
}credentials;

typedef struct {
    name s_name;
    address s_address;
    char phone_no[11];

}seller;

typedef struct {
    int u_ID;
    int p_ID;
    int o_ID;
    int o_p_ID;
}counters;

counters main_counter={0,0,0,0};
counters id_counter={0,0,0,0};

typedef struct {
    int u_ID;
    name u_name;
    address u_address;
    int prime;
    char phone_no[11];
    char email_id[20];
    char password[20];
    int i;
    int o_ID[100];
    int nooforders;
}user;

typedef struct{
    int p_ID;
    char p_name[30];
    char descr[100];
    int no_of_products;
    float price;
    float rating;
    int daystodeliver;
    int daysleft;
   // int discount;
    seller p_seller;
}product;

typedef struct{
    int o_ID;
    int u_ID;
    int p_ID;
    user o_user;
    product o_product;
    dat o_date;
}order;

struct product_list{
    product data;
    struct product_list *next;
    struct product_list *prev;
};

struct order_list{
    order data;
    struct order_list *next;
    struct order_list *prev;
};

user currentusr;

typedef struct order_list ORDER;
ORDER *order_front =NULL, *order_end = NULL, *order_temp = NULL;
ORDER *order_p_front = NULL, *order_p_end = NULL, *order_p_temp = NULL;

int monthDays[12] = {31, 28, 31, 30, 31, 30,
                           31, 31, 30, 31, 30, 31};
typedef struct product_list PRODUCT;
PRODUCT *product_front =NULL, *product_end=NULL, *product_temp=NULL;
dat curdate;
time_t t;
struct tm *tm;
void insert_product_tolist()
{
    if(product_front == NULL)
    {
        product_front = product_temp;
        product_end = product_temp;
    }
    else
    {
        product_end->next = product_temp;
        product_temp->prev = product_end;
        product_end = product_temp;
    }
}

struct user_list{
    user data;
    struct user_list *next;
    struct user_list *prev;
};

typedef struct user_list USER;

USER* get_user()
{
    USER *temp;
    temp = (USER*)malloc(sizeof(USER));
    if(temp == NULL)
    {
        printf("Insufficient memory\n");
        exit(EXIT_FAILURE);
    }
    return temp;
};

void write_counters(counters c)
{
    FILE *write = fopen("data/counters.dat","w");
    if(write == NULL)
    {
        printf("File couldn't be created\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&c,sizeof(counters),1,write);
    fclose(write);
}
void write_idcounters(counters c)
{
    FILE *write = fopen("data/idcounters.dat","w");
    if(write == NULL)
    {
        printf("File couldn't be created\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&c,sizeof(counters),1,write);
    fclose(write);
}

void read_counters()
{
    FILE *read = fopen("data/counters.dat","r");
    if(read == NULL)
    {
        printf("File doesn't exist\n");
        exit(EXIT_FAILURE);
    }
    fread(&main_counter,sizeof(counters),1,read);
    fclose(read);
}

void read_idcounters()
{
    FILE *read = fopen("data/idcounters.dat","r");
    if(read == NULL)
    {
        printf("File doesn't exist\n");
        exit(EXIT_FAILURE);
    }
    fread(&id_counter,sizeof(counters),1,read);
    fclose(read);
}

USER *user_front =NULL, *user_end=NULL, *user_temp=NULL;

void insert_user_tolist()
{
    if(user_front == NULL)
    {
        user_front = user_temp;
        user_end = user_temp;
    }
    else
    {
        user_end->next = user_temp;
        user_temp->prev = user_end;
        user_end = user_temp;
    }
}
void write_order_file()
{
    FILE *orders = fopen("data/order_details.dat","w");
    order t;
    order_temp = order_front;
    while(order_temp!=NULL)
    {
        t = order_temp->data;
        fwrite(&t,sizeof(order),1,orders);
        order_temp = order_temp->next;
    }
    fclose(orders);
}
void load_users()
{
    FILE *load_user=fopen("data/user_details.dat","r");
    int i = main_counter.u_ID;
    user loader;
    while(i>0)
    {
        fread(&loader,sizeof(user),1,load_user);
        user_temp = (USER*)malloc(sizeof(USER));
        user_temp->next = NULL;
        user_temp->prev = NULL;
        user_temp->data = loader;
        insert_user_tolist();
        i--;
    }
    fclose(load_user);
}

void load_products()
{
    FILE *load_product=fopen("data/product_details.dat","r");
    int i = main_counter.p_ID;
    product loader;
    while(i>0)
    {
        fread(&loader,sizeof(product),1,load_product);
        product_temp = (PRODUCT*)malloc(sizeof(PRODUCT));
        product_temp->next = NULL;
        product_temp->prev = NULL;
        product_temp->data = loader;
        insert_product_tolist();
        i--;
    }
    fclose(load_product);
}
int count_leap_years(dat d)
{
    int years = d.year;
    if(d.month<=2)
        years--;
    return years/4-years/100+years/400;
}
int difference(dat date, int daystodeliver)
{
    int i,dif;
    unsigned long int n1,n2;
    n1 = date.year*365 + date.day;
    for(i=0;i<(date.month)-1;i++)
    {
        n1+= monthDays[i];
        //printf("Yes\n");
    }
    n1 += count_leap_years(date);
    n2 = curdate.year*365 + curdate.day;
    for(i=0;i<curdate.month-1;i++)
        n2+=monthDays[i];
    n2+= count_leap_years(curdate);
    dif = n2-n1;
    if(dif >= daystodeliver)
        return 0;
    else
        return daystodeliver-dif;

}
void insert_order_tolist()
{
    if(order_front == NULL)
    {
        order_front = order_temp;
        order_end = order_temp;
    }
    else
    {
        order_end->next = order_temp;
        order_temp->prev = order_end;
        order_end = order_temp;
    }
}
void load_orders()
{
    FILE *load_order=fopen("data/order_details.dat","r");
    int i = main_counter.o_ID,noofdays;
    order loader;
    while(i>0)
    {
        fread(&loader,sizeof(order),1,load_order);
        printf("%d\n",loader.o_date.month);
        order_temp = (ORDER*)malloc(sizeof(ORDER));
        if(loader.o_product.daysleft > 0)
        {
            noofdays = difference(loader.o_date,loader.o_product.daystodeliver);
            loader.o_product.daysleft = noofdays;
        }
        //loader.o_user = updated_user(loader.o_user);
        order_temp->next = NULL;
        order_temp->prev = NULL;
        order_temp->data = loader;
        insert_order_tolist();
        i--;
    }
    fclose(load_order);
}

int login_admin()
{
    int i = 0, cnt = 0;
    char login_name[100],password[100],pass;
    char check_login[100],check_password[100];
    FILE *read_login;
    printf("Enter Login Name: ");
    fflush(stdin);
    gets(login_name);
    printf("Enter Password: ");
    fflush(stdin);
    while(1)
    {
        if(i < 0)
            i = 0;
        pass = getch();
        if(pass == 13)
            break;
        else if(pass == 8)
        {
            if(cnt > 0)
            {
                printf("\b");
                printf(" \b");
                i--;
                cnt--;
            }
        }
        else
        {
            password[i++] = pass;
            printf("*");
            cnt++;
        }
    }
    password[i] = '\0';
    if((read_login = fopen("data/admin_login.txt","r")) == NULL)
    {
        printf("Couldn't find Admin details\n");
        exit(EXIT_FAILURE);
    }
    fscanf(read_login,"%s %s",check_login,check_password);
    fclose(read_login);

    if(!strcmp(check_login,login_name) && !strcmp(check_password,password))
        return TRUE;

    return FALSE;
}

int login_user()
{
    char login_name[100],password[100],pass;
    int match = 0,i = 0, cnt = 0;
    printf("Enter Email ID: ");
    fflush(stdin);
    gets(login_name);
    printf("Enter Password: ");
    fflush(stdin);
    while(1)
    {
        if(i < 0)
            i = 0;
        pass = getch();
        if(pass == 13)
            break;
        else if(pass == 8)
        {
            if(cnt > 0)
            {
                printf("\b");
                printf(" ");
                printf("\b");
                i--;
                cnt--;
            }
        }
        else
        {
            password[i++] = pass;
            printf("*");
            cnt++;
        }
    }
    password[i] = '\0';
    user_temp = user_front;

    while(user_temp != NULL)
    {
        //printf("%s - %s\n",user_temp->data.email_id,login_name);
        //printf("%s - %s\n",user_temp->data.password,password);
        if(strcmp(user_temp->data.email_id,login_name) == 0 && strcmp(user_temp->data.password,password) == 0)
        {
            match = 1;
            //printf("%s\n",user_temp->data.email_id);
            currentusr = user_temp->data;
            break;
        }
        user_temp = user_temp->next;
    }

    if(match == 1)
        return TRUE;

    return FALSE;
}

void write_user(user u)
{
    FILE *register_user = fopen("data/user_details.dat","a");

    if(register_user == NULL)
    {
        printf("cannot create a file\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&u, sizeof(user),1,register_user);
    fclose(register_user);
}

void user_register()
{
    user usr;
    user_temp = (USER *)malloc(sizeof(USER));
    char password1[20],password2[20];
    //read_counters();
    main_counter.u_ID++;
    id_counter.u_ID++;
    usr.u_ID = id_counter.u_ID;
    printf("Enter your Details\n\n");
    printf("First Name: ");
    fflush(stdin);
    gets(usr.u_name.fname);
    fflush(stdin);
    printf("Last Name: ");
    gets(usr.u_name.lname);
    fflush(stdin);
    printf("Address\n");
    printf("Door No: ");
    gets(usr.u_address.door_no);
    fflush(stdin);
    printf("Address line 1: ");
    gets(usr.u_address.address_line1);
    fflush(stdin);
    printf("City: ");
    gets(usr.u_address.city);
    fflush(stdin);
    printf("State: ");
    gets(usr.u_address.state);
    fflush(stdin);
    printf("Enter Email ID: ");
    gets(usr.email_id);
    printf("Enter phone number: ");
    gets(usr.phone_no);
    fflush(stdin);
    reenter_password:
    printf("Password: ");
    gets(password1);
    fflush(stdin);
    printf("Reenter Password: ");
    gets(password2);
    fflush(stdin);
    usr.nooforders = 0;

    if(strcmp(password1,password2) != 0)
    {
        printf("Passwords dont match Enter again\n");
        goto reenter_password;
    }
    strcpy(usr.password,password1);
    usr.prime = 0;
    user_temp->data = usr;
    user_temp->next = NULL;
    user_temp->prev = NULL;
    insert_user_tolist();
    write_counters(main_counter);
    write_user(usr);
    printf("Registration Successful\n");
    system("pause");
}

void view_users()
{
    user_temp = user_front;
    printf("USER ID \tUSER NAME \tEMAIL ID\n");

    while(user_temp != NULL)
    {
        printf("%4d\t\t",user_temp->data.u_ID);
        printf("%7s\t",user_temp->data.u_name.fname);
        printf("%s\n",user_temp->data.email_id);
        user_temp = user_temp->next;
    }
    system("pause");
}

void displayele(USER *ptr)
{
    printf("DETAILS\n");
    printf("-------\n");
    printf("User ID: %d\n",ptr->data.u_ID);
    printf("User Name: %s %s\n",ptr->data.u_name.fname, ptr->data.u_name.lname);
    printf("Email ID: %s\n",ptr->data.email_id);
    printf("Address: %s %s %s %s\n",ptr->data.u_address.door_no, ptr->data.u_address.address_line1, ptr->data.u_address.address_line1,ptr->data.u_address.address_line1);
    printf("Phone Number: %s\n",ptr->data.phone_no);
    printf("\n");
}

int deleteinlist(int uid)
{
    int i;
    char ch;
    user_temp = user_front;

    if(user_front == NULL)
        return FALSE;
    else if(user_front->next == NULL)
    {
        if(user_front->data.u_ID == uid)
        {
            displayele(user_front);
            printf("Do you wanna delete the above user(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);
            if(ch == 'Y' || ch =='y')
            {
                user_front = NULL;
                user_end = NULL;
                for(i=0;i<user_temp->data.nooforders;i++)
                {
                    delete_in_p(user_temp->data.o_ID[i]);
                    delete_in_order(user_temp->data.o_ID[i]);
                    rewriteorders();
                }
                free(user_temp);
                return TRUE;
            }
        }
    }
    else
    {
        if(user_front->data.u_ID == uid)
        {
            displayele(user_front);
            printf("Do you wanna delete the above user(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);

            if(ch=='Y' || ch=='y')
            {
                user_temp = user_front;
                user_front = user_front->next;
                user_front->prev = NULL;
                for(i=0;i<user_temp->data.nooforders;i++)
                {
                    delete_in_p(user_temp->data.o_ID[i]);
                    delete_in_order(user_temp->data.o_ID[i]);
                    rewriteorders();
                }
                free(user_temp);
                return TRUE;
            }

        }
        else if(user_end->data.u_ID == uid)
        {
            displayele(user_end);
            printf("Do you wanna delete the above user(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);
            if(ch == 'Y' || ch=='y')
            {
                user_temp = user_end;
                user_end = user_end->prev;
                user_end->next = NULL;
                for(i=0;i<user_temp->data.nooforders;i++)
                {
                    delete_in_p(user_temp->data.o_ID[i]);
                    delete_in_order(user_temp->data.o_ID[i]);
                    rewriteorders();
                }
                free(user_temp);
                return TRUE;
            }
        }
        else
        {
            while(user_temp!=NULL)
            {
                if(user_temp->data.u_ID == uid)
                {
                    displayele(user_temp);
                    printf("Do you wanna delete the above user(Y/N)");
                    fflush(stdin);
                    scanf("%c",&ch);
                    if(ch == 'Y' || ch =='y')
                    {
                        user_temp->prev->next = user_temp->next;
                        user_temp->next->prev = user_temp->prev;
                        for(i=0;i<user_temp->data.nooforders;i++)
                        {
                            delete_in_p(user_temp->data.o_ID[i]);
                            delete_in_order(user_temp->data.o_ID[i]);
                            rewriteorders();
                        }
                        free(user_temp);
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

void deleteinfile(int uid)
{
    FILE *user_data = fopen("data/user_details.dat","w");
    USER *temporary;
    user t2;
    temporary = user_front;

    while(temporary!= NULL)
    {
        t2 = temporary->data;
        fwrite(&t2,sizeof(user),1,user_data);
        temporary = temporary->next;
    }
    fclose(user_data);
}

void write_product(product p)
{
    FILE *register_product = fopen("data/product_details.dat","a");

    if(register_product == NULL)
    {
        printf("cannot created a file\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&p, sizeof(product),1,register_product);
    fclose(register_product);
}

void product_register()
{
    product pd;
    product_temp = (PRODUCT*)malloc(sizeof(PRODUCT));
    //read_counters();
    main_counter.p_ID++;
    id_counter.p_ID++;
    pd.p_ID = id_counter.p_ID;
    printf("Enter product Details\n\n");
    printf("Product Name: ");
    fflush(stdin);
    gets(pd.p_name);
    printf("Describe the Product\n");
    fflush(stdin);
    gets(pd.descr);
    printf("Product Remaining: ");
    fflush(stdin);
    scanf("%d",&pd.no_of_products);
    printf("Enter the Price of Product: ");
    fflush(stdin);
    scanf("%f",&pd.price);
    printf("Enter number of days to deliver: ");
    fflush(stdin);
    scanf("%d",&pd.daystodeliver);
    pd.rating=0.0;
    printf("Enter the Seller First Name: ");
    fflush(stdin);
    gets(pd.p_seller.s_name.fname);
    fflush(stdin);
    printf("Enter the Seller Last Name: ");
    gets(pd.p_seller.s_name.lname);
    fflush(stdin);
    printf("Enter the Seller Address\n");
    printf("Door No: ");
    gets(pd.p_seller.s_address.door_no);
    fflush(stdin);
    printf("Address line 1: ");
    gets(pd.p_seller.s_address.address_line1);
    fflush(stdin);
    printf("City: ");
    gets(pd.p_seller.s_address.city);
    fflush(stdin);
    printf("State: ");
    gets(pd.p_seller.s_address.state);
    fflush(stdin);
    printf("Enter phone number: ");
    gets(pd.p_seller.phone_no);
    fflush(stdin);
    pd.daysleft = pd.daystodeliver;
    product_temp->data = pd;
    product_temp->next = NULL;
    product_temp->prev = NULL;
    insert_product_tolist();
    write_counters(main_counter);
    write_idcounters(id_counter);
    write_product(pd);
    printf("Product Adding Successful\n");
    system("pause");
}

void view_product()
{
    product_temp = product_front;
    printf("Product ID \tProduct NAME \tRemaining Products \tPrice \n");

    while(product_temp != NULL)
    {
        printf("%5d\t\t",product_temp->data.p_ID);
        printf("%9s\t",product_temp->data.p_name);
        printf("%11d\t\t",product_temp->data.no_of_products);
        printf("%8.2f\n",product_temp->data.price);
        //printf("%5f\t",product_temp->data.rating);
        product_temp = product_temp->next;
    }
    system("pause");
}

void displaypele(PRODUCT *ptr)
{
    printf("DETAILS\n");
    printf("-------\n");
    printf("Product ID: %d\n",ptr->data.p_ID);
    printf("Product Name: %s \n",ptr->data.p_name);
    printf("Price: %f\n",ptr->data.price);
    printf("Seller Name: %s %s \n",ptr->data.p_seller.s_name.fname,ptr->data.p_seller.s_name.lname);
    //need to show descp ,rating
    //seller details?
    printf("\n");
}

int deleteproductinlist(int pid)
{
    char ch;
    product_temp = product_front;

    if(product_front == NULL)
        return FALSE;
    else if(product_front->next == NULL)
    {
        if(product_front->data.p_ID == pid)
        {
            displaypele(product_front);
            printf("Do you wanna delete the above Product(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);
            if(ch == 'Y' || ch =='y')
            {
                product_front = NULL;
                product_end = NULL;
                free(product_temp);
                return TRUE;
            }
        }

    }
    else
    {
        if(product_front->data.p_ID == pid)
        {
            displaypele(product_front);
            printf("Do you wanna delete the above Product(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);
            if(ch=='Y' || ch=='y')
            {
                product_temp = product_front;
                product_front = product_front->next;
                product_front->prev = NULL;
                free(product_temp);
                return TRUE;
            }

        }
        else if(product_end->data.p_ID == pid)
        {
            displaypele(product_end);
            printf("Do you wanna delete the above Product(Y/N)");
            fflush(stdin);
            scanf("%c",&ch);
            if(ch == 'Y' || ch=='y')
            {
                product_temp = product_end;
                product_end = product_end->prev;
                product_end->next = NULL;
                free(product_temp);
                return TRUE;
            }
        }
        else
        {
            while(product_temp!=NULL)
            {
                if(product_temp->data.p_ID == pid)
                {
                    displaypele(product_temp);
                    printf("Do you wanna delete the above Product(Y/N)");
                    fflush(stdin);
                    scanf("%c",&ch);
                    if(ch == 'Y' || ch =='y')
                    {
                        product_temp->prev->next = product_temp->next;
                        product_temp->next->prev = product_temp->prev;
                        free(product_temp);
                        return TRUE;
                    }
                }
            }
        }
    }

    return FALSE;
}

void deleteproductinfile(int pid)
{
    FILE *product_data = fopen("data/product_details.dat","w");
    PRODUCT *temporary;
    product t2;
    temporary = product_front;

    while(temporary!= NULL)
    {
        t2 = temporary->data;
        fwrite(&t2,sizeof(product),1,product_data);
        temporary = temporary->next;
    }
    fclose(product_data);
}
int view_user_details_by_ID(int id)
{
    user_temp = user_front;
    while(user_temp!=NULL)
    {
        if(user_temp->data.u_ID == id)
        {
            printf("\t\t USER DETAILS of %d\n",id);
            printf("\t\t-------------------\n");
            printf("Name: %s %s\n",user_temp->data.u_name.fname,user_temp->data.u_name.lname);
            printf("Address: %s, %s\n",user_temp->data.u_address.door_no,user_temp->data.u_address.address_line1);
            printf("         %s, %s\n",user_temp->data.u_address.city,user_temp->data.u_address.state);
            printf("Phone Number: %s\n",user_temp->data.phone_no);
            printf("Email-ID: %s\n",user_temp->data.email_id);
            printf("Password: %s\n",user_temp->data.password);
            printf("Prime Number: %d\n",user_temp->data.prime);
            printf("Total number of orders placed: %d\n",user_temp->data.nooforders);
            return TRUE;
        }
        user_temp = user_temp->next;
    }
    return FALSE;
}

void manage_users()
{
    int ch,uid,res;
    while(1)
    {
        system("cls");
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. Add User\n2. View Users\n3. Delete User by ID\n4. View User detail by ID\n5. Exit\n\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                user_register();
                break;
            case 2:
                view_users();
                break;
            case 3:
                printf("Enter User ID to Delete: ");
                scanf("%d",&uid);
                res = deleteinlist(uid);

                if(res)
                {
                    printf("Deleted User\n");
                    deleteinfile(uid);
                    main_counter.u_ID--;
                    write_counters(main_counter);
                }
                else
                {
                    printf("Couldn't delete the above user\n");
                }
                break;
            case 4:
                printf("Enter user ID: ");
                scanf("%d",&uid);
                if(!view_user_details_by_ID(uid))
                    printf("User Details not found.\n");
                system("pause");
                break;
            case 5:
                return;
            default:
                printf("Wrong Choice entered\n");
                system("pause");
        }
    }
}
int display_product_by_id(int id)
{
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        if(product_temp->data.p_ID == id)
        {
            printf("\t\t PRODUCT DETAILS of %d\n",id);
            printf("\t\t----------------------\n");
            printf("Product ID: %d\n",product_temp->data.p_ID);
            printf("Product Name: %s\n",product_temp->data.p_name);
            printf("Product Description\n");
            printf("%s\n",product_temp->data.descr);
            printf("Number of products: %d\n",product_temp->data.no_of_products);
            printf("Days to Deliver: %d\n",product_temp->data.daystodeliver);
            printf("\nSELLER DETAILS\n");
            printf("--------------\n");
            printf("Seller Name: %s %s\n",product_temp->data.p_seller.s_name.fname, product_temp->data.p_seller.s_name.lname);
            printf("Seller Address: %s, %s\n",product_temp->data.p_seller.s_address.door_no,product_temp->data.p_seller.s_address.address_line1);
            printf("                %s, %s\n",product_temp->data.p_seller.s_address.city,product_temp->data.p_seller.s_address.state);
            printf("Phone Number: %s\n",product_temp->data.p_seller.phone_no);
            return TRUE;
        }
        product_temp = product_temp->next;
    }
    return FALSE;
}
int display_product_by_name(char name[])
{
    int i;
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        if(strcmpi(name,product_temp->data.p_name) == 0)
        {
            printf("\t\tPRODUCT DETAILS of %s\n",name);
            printf("\t\t-------------------");
            for(i=0;i<strlen(name);i++)
                printf("-");
            printf("\n");
            printf("Product ID: %d\n",product_temp->data.p_ID);
            printf("Product Name: %s\n",product_temp->data.p_name);
            printf("Product Description\n");
            printf("%s\n",product_temp->data.descr);
            printf("Number of products: %d\n",product_temp->data.no_of_products);
            printf("Days to Deliver: %d\n",product_temp->data.daystodeliver);
            printf("\nSELLER DETAILs\n");
            printf("--------------\n");
            printf("Seller Name: %s %s\n",product_temp->data.p_seller.s_name.fname, product_temp->data.p_seller.s_name.lname);
            printf("Seller Address: %s, %s\n",product_temp->data.p_seller.s_address.door_no,product_temp->data.p_seller.s_address.address_line1);
            printf("                %s, %s\n",product_temp->data.p_seller.s_address.city,product_temp->data.p_seller.s_address.state);
            printf("Phone Number: %s\n",product_temp->data.p_seller.phone_no);
            return TRUE;
        }
        product_temp = product_temp->next;
    }
    return FALSE;
}
void view_order(ORDER *ptr)
{
    //order_temp = order_front;
    printf("ORDER ID \tPRODUCT ID  \tUSER ID \tORDER DATE \tDAYS LEFT TO DELIVER\n");
    while(ptr != NULL)
    {
        printf("%5d\t\t%6d\t%12d\t\t",ptr->data.o_ID,ptr->data.p_ID,ptr->data.u_ID);
        printf("%d/%d/%d",ptr->data.o_date.day,ptr->data.o_date.month, ptr->data.o_date.year);
        if(ptr->data.o_product.daysleft > 0)
            printf("\t\t%4d\n",ptr->data.o_product.daysleft);
        else
            printf("\t     DELIVERED\n");
        ptr = ptr->next;
    }
    system("pause");
}
void update_product(int pid,product p)
{
    product_temp = product_front;
    while(product_temp != NULL)
    {
        if(product_temp->data.p_ID == pid)
        {
            product_temp->data = p;
            break;
        }
        product_temp = product_temp->next;
    }
}
void edit_product(int pid)
{
    product pd;
    pd.p_ID = pid;
    printf("Enter product Details\n\n");
    printf("Product Name: ");
    fflush(stdin);
    gets(pd.p_name);
    printf("Describe the Product\n");
    fflush(stdin);
    gets(pd.descr);
    printf("Product Remaining: ");
    fflush(stdin);
    scanf("%d",&pd.no_of_products);
    printf("Enter the Price of Product: ");
    fflush(stdin);
    scanf("%f",&pd.price);
    printf("Enter number of days to deliver: ");
    fflush(stdin);
    scanf("%d",&pd.daystodeliver);
    printf("Enter the Seller First Name: ");
    fflush(stdin);
    gets(pd.p_seller.s_name.fname);
    fflush(stdin);
    printf("Enter the Seller Last Name: ");
    gets(pd.p_seller.s_name.lname);
    fflush(stdin);
    printf("Enter the Seller Address\n");
    printf("Door No: ");
    gets(pd.p_seller.s_address.door_no);
    fflush(stdin);
    printf("Address line 1: ");
    gets(pd.p_seller.s_address.address_line1);
    fflush(stdin);
    printf("City: ");
    gets(pd.p_seller.s_address.city);
    fflush(stdin);
    printf("State: ");
    gets(pd.p_seller.s_address.state);
    fflush(stdin);
    printf("Enter phone number: ");
    gets(pd.p_seller.phone_no);
    fflush(stdin);
    pd.daysleft = pd.daystodeliver;
    update_product(pid, pd);
    deleteproductinfile(pid);//does the required function to update in file
    printf("Product updated successfully.\n");
    system("pause");
}
void manage_products()
{
    int ch,pid,res;
    char p_name[30];
    while(1)
    {
        system("cls");
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. Add Products\n2. View Products\n3. Delete Products by ID\n4. View Products detail by Name\n5. Edit product details by ID\n6. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch);

        switch(ch)
        {
            case 1:
                product_register();
                break;
            case 2:
                view_product();
                break;
            case 3:
                printf("Enter Product ID to Delete: ");
                scanf("%d",&pid);
                res = deleteproductinlist(pid);

                if(res)
                {
                    printf("Deleted Product\n");
                    deleteproductinfile(pid);
                    main_counter.p_ID--;
                    write_counters(main_counter);
                }
                else
                {
                    printf("Couldn't delete the above Product\n");
                }
                break;
            case 4:
                printf("Enter Product Name to search: ");
                fflush(stdin);
                gets(p_name);
                if(!display_product_by_name(p_name))
                    printf("Product Details not found\n");
                system("pause");
                break;
            case 5:
                //enteragain:
                printf("Enter product ID to edit: ");
                fflush(stdin);
                scanf("%d",&pid);
                if(!check_productp(pid))
                {
                    printf("Wrong product ID entered.\n");
                    //goto enteragain;
                }
                edit_product(pid);
                break;
            case 6:
                return;
            default:
                printf("Wrong Choice entered\n");
                system("pause");
        }
    }
}
int display_order_details(int id)
{
    int psuc,usuc;
    order_temp = order_front;
    while(order_temp != NULL)
    {
        if(order_temp->data.o_ID == id)
        {
            printf("\n\n");
            printf("\t\tORDER DETAILS\n");
            printf("\t\t-------------\n");
            printf("Order ID: %d\n",id);
            printf("USER ID: %d\n",order_temp->data.u_ID);
            printf("PRODUCT ID: %d\n",order_temp->data.p_ID);
            printf("Order Date: %d/%d/%d\n",order_temp->data.o_date.day,order_temp->data.o_date.month,order_temp->data.o_date.year);
            printf("\nUSER DETAILS\n");
            printf("------------\n");
            usuc = view_user_details_by_ID(order_temp->data.u_ID);
            printf("\nPRODUCT DETAILS\n");
            printf("---------------\n");
            psuc = display_product_by_id(order_end->data.p_ID);
            return TRUE;
        }
        order_temp = order_temp->next;
    }
    return FALSE;
}
int delete_in_p(int oid)
{
    order_p_temp = order_p_front;
    if(order_p_temp == NULL)
        return FALSE;
    else if(order_p_front->next == NULL)
    {
        if(order_p_front->data.o_ID == oid)
        {
            order_p_front = NULL;
            order_p_end = NULL;
            free(order_p_temp);
            return TRUE;
        }
    }
    else
    {
        if(order_p_front->data.o_ID == oid)
        {
            order_p_front = order_p_front->next;
            order_p_front->prev = NULL;
            free(order_p_temp);
            return TRUE;
        }
        else if(order_p_end->data.o_ID == oid)
        {
            order_p_temp = order_p_end;
            order_p_end = order_p_end->prev;
            order_p_end->next = NULL;
            free(order_p_temp);
            return TRUE;
        }
        else
        {
            while(order_p_temp != NULL)
            {
                if(order_p_temp->data.o_ID == oid)
                {
                    order_p_temp->prev->next = order_p_temp->next;
                    order_p_temp->next->prev = order_p_temp->prev;
                    free(order_p_temp);
                    return TRUE;
                }
                order_p_temp = order_p_temp->next;
            }
        }
    }
    return FALSE;
}

void rewriteorders()
{
    FILE *order_details = fopen("data/order_details.dat","w");
    ORDER *temp;
    order o;
    temp = order_front;
    while(temp != NULL)
    {
        o = temp->data;
        fwrite(&o, sizeof(order), 1, order_details);
        temp = temp->next;
    }
    fclose(order_details);
    order_details = fopen("data/priority_order.dat","w");
    temp = order_p_front;
    while(temp != NULL)
    {
        o = temp->data;
        fwrite(&o, sizeof(order), 1, order_details);
        temp = temp->next;
    }
    fclose(order_details);
}


int delete_in_order(int oid)
{
    order_temp = order_front;
    if(order_temp == NULL)
        return FALSE;
    else if(order_front->next == NULL)
    {
        if(order_front->data.o_ID == oid)
        {
            order_front = NULL;
            order_end = NULL;
            free(order_temp);
            return TRUE;
        }
    }
    else
    {
        if(order_front->data.o_ID == oid)
        {
            order_front = order_front->next;
            order_front->prev = NULL;
            free(order_temp);
            return TRUE;
        }
        else if(order_end->data.o_ID == oid)
        {
            order_temp = order_end;
            order_end = order_end->prev;
            order_end->next = NULL;
            free(order_temp);
            return TRUE;
        }
        else
        {
            while(order_temp != NULL)
            {
                if(order_temp->data.o_ID == oid)
                {
                    order_temp->prev->next = order_temp->next;
                    order_temp->next->prev = order_temp->prev;
                    free(order_temp);
                    return TRUE;
                }
                order_temp = order_temp->next;
            }
        }
    }
    return FALSE;
}
void delete_in_user(int uid, int oid)
{
    int i;
    user_temp = user_front;
    while(user_temp != NULL)
    {
        if(user_temp->data.u_ID == uid)
        {
            for(i=0;i<user_temp->data.nooforders;i++)
            {
                if(user_temp->data.o_ID[i] == oid)
                    break;
            }
            user_temp->data.nooforders--;
            user_temp->data.prime = user_temp->data.nooforders/6;
            for(;i<user_temp->data.nooforders;i++)
            {
                user_temp->data.o_ID[i] = user_temp->data.o_ID[i+1];
            }
        }
        user_temp = user_temp->next;
    }
}
void update_noofproducts(int pid)
{
    product_temp = product_front;
    while(product_temp != NULL)
    {
        if(product_temp->data.p_ID == pid)
        {
            product_temp->data.no_of_products++;
            break;
        }
        product_temp = product_temp->next;
    }
}
void delete_order(int oid)
{
    char choice;
    int found_in_priority = 0, found_in_order = 0, res1, res2, uid,pid;
    order_p_temp = order_p_front;
    while(order_p_temp != NULL)
    {
        if(order_p_temp->data.o_ID == oid)
        {
            found_in_priority = 1;
            pid = order_p_temp->data.p_ID;
            uid = order_p_temp->data.u_ID;
        }
        order_p_temp = order_p_temp->next;
    }
    order_temp = order_front;
    while(order_temp != NULL)
    {
        if(order_temp->data.o_ID == oid)
        {
            found_in_order = 1;
        }
        order_temp = order_temp->next;
    }
    if(found_in_priority == 0 && found_in_order == 1)
    {
        printf("Order already delivered, Cannot delete it\n");
        return;
    }
    if(found_in_order == 0)
    {
        printf("Wrong Order ID, Please enter correct ID\n");
        return;
    }
    display_order_details(oid);
    printf("Do you want to Delete the above order?(Y/N)\n");
    fflush(stdin);
    scanf("%c",&choice);
    if(choice == 'Y' || choice == 'y')
    {
        res1 = delete_in_p(oid);
        res2 = delete_in_order(oid);
        if(res1 && res2)
        {
            printf("Successfully deleted Order %d\n",oid);
            rewriteorders();
            main_counter.o_ID--;
            main_counter.o_p_ID--;
            update_noofproducts(pid);
            deleteproductinfile(pid);
            write_counters(main_counter);
            delete_in_user(uid, oid);
            deleteinfile(uid);
        }
    }
}

void manage_orders()
{
    int ch,oid,res;
    char p_name[30];
    while(1)
    {
        system("cls");
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. View all current orders\n2. View all placed orders\n3. View details by Order ID\n4. Delete Order by ID\n5. Exit\n");
        //Delete order, just an option
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch);

        switch(ch)
        {
            case 1:
                view_order(order_p_front);
                break;
            case 2:
                view_order(order_front);
                break;
            case 3:
                printf("Enter your Order to search: ");
                fflush(stdin);
                scanf("%d",&oid);
                if(!display_order_details(oid))
                    printf("Order Details not found.\n");
                system("pause");
                break;
            case 4:
                printf("Enter Order ID to be deleted: ");
                fflush(stdin);
                scanf("%d",&oid);
                delete_order(oid);
                system("pause");
                break;
            case 5:
                return;
            default:
                printf("Wrong Choice entered\n");
                system("pause");
        }
    }
}

void admin_home()
{
    int ch;

    while(1)
    {
        system("cls");
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. Manage Users\n2. Manage Products\n3. Manage Orders\n4. Logout\n\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                manage_users();
                break;
            case 2:
                manage_products();
                break;
            case 3:
                manage_orders();
                break;
            case 4:
                return;
            default:
                printf("Wrong option Entered, Try again.\n");
                system("pause");
        }
    }
}
void update_current_user(int o_id)
{
    currentusr.o_ID[currentusr.nooforders] = o_id;
    currentusr.nooforders++;
    currentusr.prime = currentusr.nooforders/6;
    user_temp = user_front;
    while(user_temp!= NULL)
    {
        if(user_temp->data.u_ID == currentusr.u_ID)
        {
            user_temp->data.o_ID[user_temp->data.nooforders] = o_id;
            user_temp->data.nooforders++;
            user_temp->data.prime = currentusr.prime;
        }
        user_temp = user_temp->next;
    }
}
void write_user_file()
{
    FILE *users = fopen("data/user_details.dat","w");
    user t;
    user_temp = user_front;
    while(user_temp!=NULL)
    {
        t = user_temp->data;
        fwrite(&t,sizeof(user),1,users);
        user_temp = user_temp->next;
    }
    fclose(users);
}
void write_product_file()
{
    FILE *products = fopen("data/product_details.dat","w");
    product t;
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        t = product_temp->data;
        fwrite(&t,sizeof(product),1,products);
        product_temp = product_temp->next;
    }
    fclose(products);
}
void write_order(order o)
{
    FILE *register_orders = fopen("data/order_details.dat","a");
    if(register_orders == NULL)
    {
        printf("cannot create a file\n");
        exit(EXIT_FAILURE);
    }
    fwrite(&o, sizeof(order), 1, register_orders);
    fclose(register_orders);
    //printf("Wrote Order to Order_details.dat\n");
}
int check_productp(int p_id)
{
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        if(product_temp->data.p_ID == p_id)
        {
            return TRUE;
        }
        product_temp = product_temp->next;
    }
    return FALSE;
}
int check_product(int p_id)
{
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        if(product_temp->data.p_ID == p_id && product_temp->data.no_of_products > 0)
        {
            product_temp->data.no_of_products--;
            write_product_file();
            return TRUE;
        }
        product_temp = product_temp->next;
    }
    return FALSE;
}
product return_product(int p_id)
{
    product t;
    product_temp = product_front;
    while(product_temp!=NULL)
    {
        if(product_temp->data.p_ID == p_id)
        {
            t = product_temp->data;
            return t;
        }
        product_temp = product_temp->next;
    }
}
void writefilepq()
{
    order temppq;
    FILE *orderpq = fopen("data/priority_order.dat","w");
    if(orderpq == NULL)
    {
        printf("File can't be created.\n");
        exit(EXIT_FAILURE);
    }
    order_p_temp = order_p_front;
    while(order_p_temp!= NULL)
    {
        temppq = order_p_temp->data;
        fwrite(&temppq, sizeof(order), 1, orderpq);
        order_p_temp = order_p_temp->next;
    }
    fclose(orderpq);
}
void write_p_order(order opq)
{
    FILE *writeopq = fopen("data/priority_order.dat","a");
    if(writeopq == NULL)
    {
        printf("File couldn't be created");
        exit(EXIT_FAILURE);
    }
    fwrite(&opq, sizeof(order), 1, writeopq);
    fclose(writeopq);
}
void write_p_q()
{
    ORDER *ot;
    if(order_p_front == NULL)
    {
        order_p_front = order_p_temp;
        order_p_end = order_p_temp;
    }
    else
    {
        ot = order_p_front;
        while(ot != NULL && ot->data.o_user.prime >= order_p_temp->data.o_user.prime)
            ot = ot->next;
        if(ot == order_p_front)
        {
            order_p_front->prev = order_p_temp;
            order_p_temp->next = order_p_front;
            order_p_front = order_p_temp;
        }
        else if(ot!= NULL)
        {
            ot->prev->next = order_p_temp;
            order_p_temp->prev = ot->prev;
            order_p_temp->next = ot;
            ot->prev = order_p_temp;
        }
        else
        {
            order_p_end->next = order_p_temp;
            order_p_temp->prev = order_p_end;
            order_p_end = order_p_temp;
        }
    }
    write_p_order(order_p_temp->data);
    main_counter.o_p_ID++;
    write_counters(main_counter);
}
void insert_poq_tolist()
{
    if(order_p_front == NULL)
    {
        order_p_front = order_p_temp;
        order_p_end = order_p_temp;
    }
    else
    {
        order_p_end->next = order_p_temp;
        order_p_temp->prev = order_p_end;
        order_p_end = order_p_temp;
    }
}
order updated_order(order loader)
{
    int dif;

    dif = difference(loader.o_date, loader.o_product.daystodeliver);
    loader.o_product.daysleft = dif;
    return loader;
}
int checkorder(order test)
{
    int dif;
    dif = difference(test.o_date,test.o_product.daystodeliver);
    printf("Yes\n");
    if(dif == 0)
        return 0;
    else
        return 1;
}
void load_p_orders()
{
    FILE *load_order = fopen("data/priority_order.dat","r");
    int i = main_counter.o_p_ID;
    order loader;
    while(i>0)
    {
        fread(&loader, sizeof(order), 1, load_order);
        printf("%d\n",loader.o_date.month);
        if(checkorder(loader))
        {
            printf("Yes\n");
            loader = updated_order(loader);
            order_p_temp = (ORDER*)malloc(sizeof(ORDER));
            order_p_temp->data = loader;
            order_p_temp->next = NULL;
            order_p_temp->prev = NULL;
            insert_poq_tolist();
        }
        i--;
    }
    fclose(load_order);

}
void place_order()
{
    int p_id;
    order od;
    p_id_again:
    printf("Enter Product ID to Place order: ");
    scanf("%d",&p_id);
    if(!check_product(p_id))
    {
        printf("Product not Available.\n");
        goto p_id_again;
    }
    order_temp = (ORDER*)malloc(sizeof(ORDER));
    order_p_temp = (ORDER*)malloc(sizeof(ORDER));
    main_counter.o_ID++;
    id_counter.o_ID++;
    od.o_ID = id_counter.o_ID;
    od.p_ID = p_id;
    od.u_ID = currentusr.u_ID;
    update_current_user(od.o_ID);
    write_user_file();
    write_counters(main_counter);
    write_idcounters(id_counter);
    od.o_user = currentusr;
    od.o_product = return_product(p_id);
    od.o_product.daystodeliver = ceil(od.o_product.daystodeliver - (0.5 * od.o_user.prime));
    od.o_product.daysleft = od.o_product.daystodeliver;
    od.o_date = curdate;
    order_temp->data = od;
    order_temp->next = NULL;
    order_temp->prev = NULL;
    order_p_temp->data = od;
    order_p_temp->next = NULL;
    order_p_temp->prev = NULL;
    write_p_q();
    insert_order_tolist();
    write_order(od);
    printf("Order placed successfully\n");
    system("pause");
}

void display_order(order data)
{
    //fill the printf statements here
    printf("%5d\t\t%9s\t%d/%d/%d",data.o_ID,data.o_product.p_name,data.o_date.day,data.o_date.month,data.o_date.year);
    if(data.o_product.daysleft > 0)
        printf("\t\t%4d\n",data.o_product.daysleft);
    else
        printf("\t     DELIVERED\n");
}

void view_user_orders()
{
    int i = currentusr.nooforders,j=0;
    printf("ORDER ID \tPRODUCT NAME \tORDER DATE \tDAYS LEFT TO DELIVER \n");
    while(i>0)
    {
        order_temp = order_front;
        while(order_temp!=NULL)
        {
            if(order_temp->data.o_ID == currentusr.o_ID[j])
            {
                display_order(order_temp->data);
            }
            order_temp=order_temp->next;
        }
        j++;
        i--;
    }
    system("pause");
}
user current_usr(int uid)
{
    user_temp = user_front;
    while(user_temp != NULL)
    {
        if(user_temp->data.u_ID == uid)
            return user_temp->data;

    }
}

int check_order(int oid)
{
    int i;
    for(i=0;i<currentusr.nooforders;i++)
    {
        if(currentusr.o_ID[i] == oid)
            return TRUE;
    }
    return FALSE;
}
void update_user()
{
    user_temp = user_front;
    while(user_temp != NULL)
    {
        if(user_temp->data.u_ID == currentusr.u_ID)
        {
            user_temp->data = currentusr;
            break;
        }
        user_temp = user_temp->next;
    }
}
void edit_user()
{
    char password1[20],password2[20];
    printf("Enter your Details\n\n");
    printf("First Name: ");
    fflush(stdin);
    gets(currentusr.u_name.fname);
    printf("Last Name: ");
    gets(currentusr.u_name.lname);
    fflush(stdin);
    printf("Address\n");
    printf("Door No: ");
    gets(currentusr.u_address.door_no);
    fflush(stdin);
    printf("Address line 1: ");
    gets(currentusr.u_address.address_line1);
    fflush(stdin);
    printf("City: ");
    gets(currentusr.u_address.city);
    fflush(stdin);
    printf("State: ");
    gets(currentusr.u_address.state);
    fflush(stdin);
    printf("Enter Email ID: ");
    gets(currentusr.email_id);
    printf("Enter phone number: ");
    gets(currentusr.phone_no);
    fflush(stdin);
    reenter_password:
    printf("Password: ");
    gets(password1);
    fflush(stdin);
    printf("Reenter Password: ");
    gets(password2);
    if(strcmp(password1,password2) != 0)
    {
        printf("Passwords don't match Enter again\n");
        goto reenter_password;
    }
    strcpy(currentusr.password,password1);
    update_user();
    deleteinfile(currentusr.u_ID);//function does the same job required
    printf("User details updated successfully.\n");
    system("pause");
}
void user_home()
{
    char choice;
    int ch,oid;

    while(1)
    {
        system("cls");
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. View Product\n2. Place Order by Product ID\n3. View Orders\n4. Delete Order by ID\n5. Edit Details\n6. Delete Account\n7. Logout\n\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:
                view_product();
                break;
            case 2:
                place_order();
                break;
            case 3:
                view_user_orders();
                break;
            case 4:
                printf("Enter Order ID to be deleted: ");
                scanf("%d",&oid);
                if(check_order(oid))
                {
                    delete_order(oid);
                    currentusr = current_usr(currentusr.u_ID);
                }
                else
                {
                    printf("Cannot delete the order which you haven't placed.\n");
                }
                break;
            case 5:
                edit_user();
                break;
            case 6:
                printf("Do you really want to delete your account?(Y/N): ");
                fflush(stdin);
                scanf("%c",&choice);
                if(choice == 'Y' || choice == 'y')
                {
                    deleteinlist(currentusr.u_ID);
                    deleteinfile(currentusr.u_ID);
                    return;
                }
                break;
            case 7:
                //logout
                return ;
            default:
                printf("Wrong option Entered, Try again.\n");
                system("pause");
        }
    }
}

int main()
{
    int ch1,ch2,user_wrong_login=0,admin_wrong_login=0;
    t = time(NULL);
    tm = localtime(&t);
    curdate.day = tm->tm_mday;
    curdate.month = tm->tm_mon+1;
    curdate.year = tm->tm_year + 1900;
    FILE *load_impt = fopen("data/counters.dat","r");

    if(load_impt == NULL)
    {
        write_counters(main_counter);
    }
    fclose(load_impt);
    load_impt = fopen("data/idcounters.dat","r");
    if(load_impt == NULL)
        write_idcounters(id_counter);
    fclose(load_impt);
    read_idcounters();
    read_counters();
    load_users();
    load_products();
    load_orders();
    load_p_orders();
    //write_order_file();
    //load priority orders
    // write priority orders to file
    while(1)
    {
        system("cls");
        //printf("Current date: %d/%d/%d\n",curdate.day,curdate.month,curdate.year);
        printf("ONLINE SHOPPING APPLICATION\n\n");
        printf("1. Admin Login\n2. User Login/Register\n3. Exit\n");
        printf("Enter your choice: ");
        fflush(stdin);
        scanf("%d",&ch1);
        switch(ch1)
        {
            case 1:
                admin_login_again:

                if(login_admin())
                {
                    admin_wrong_login = 0;
                    admin_home();
                }
                else
                {
                    printf("\nLogin Credentials are wrong. Enter again\n");
                    admin_wrong_login++;

                    if(admin_wrong_login <= 3)
                        goto admin_login_again;
                }
                break;
            case 2:
                while(1)
                {
                    system("cls");
                    printf("ONLINE SHOPPING APPLICATION\n\n");
                    printf("1. Login\n2. Sign up\n3. Exit\n\n");
                    printf("Enter your choice: ");
                    fflush(stdin);
                    scanf("%d",&ch2);

                    switch(ch2)
                    {
                    case 1:
                        user_login_again:

                        if(login_user())
                        {
                            user_wrong_login = 0;
                            user_home();
                        }
                        else
                        {
                            printf("\nLogin credentials are wrong. Enter again\n");
                            user_wrong_login++;

                            if(user_wrong_login <= 3)
                                goto user_login_again;
                        }
                        break;
                    case 2:
                        user_register();
                        break;
                    case 3:
                        goto exit_loop;
                        //break;
                    default:
                        printf("Wrong Option entered. Try again\n");
                        system("pause");
                    }
                }
                exit_loop:
                break;
            case 3:
                exit(0);
                //break;
            default:
                printf("Wrong option Entered\n");
                system("pause");
        }
    }
    free(user_front);
    free(user_temp);
    free(user_end);
    return 0;
}
