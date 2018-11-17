struct name{
    char fname[30];
    char lname[30];
};
struct date{
    int day,month,year;
};
struct address{
    char door_no[10];
    char address_line1[100];
    char city[20];
    char state[20];
};
struct credentials{
    char login_name[100];
    char password[100];
};
struct seller{
    struct name s_name;
    struct address s_address;
    char phone_no[11];

};
struct counters{
    int u_ID;
    int p_ID;
    int o_ID;
}main_counter={0,0,0};
typedef struct {
    int u_ID;
    struct name u_name;
    struct address u_address;
    int prime;
    char phone_no[11];
    char email_id[20];
    int i;
    int o_ID[100];
}user;
typedef struct{
    int p_ID;
    char p_name[30];
    char descr[100];
    int no_of_products;
    float price;
    float rating;
    int discount;
    struct seller p_seller;
}product;
typedef struct{
    int o_ID;
    int u_ID;
    int p_ID;
    user o_user;
    product o_product;
    struct address shipping_address;
}order;
