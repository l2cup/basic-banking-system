#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define null NULL


typedef struct Transaction{
    long                transaction_id;
    long                first_account_id;
    long                second_account_id;
    long                first_user_id;
    long                second_user_id;
    float               change;
    struct Transaction *previous;
    struct Transaction *next;
}Transaction;

typedef struct Account{
    long                account_id;
    long                user_id;
    float               balance;
    float               debt_release;
    float               debt;
    Transaction        *head;             //Lista transakcija
    struct Account     *previous;
    struct Account     *next;
}Account;

typedef struct Data{
    char                name[20];
    char                surname[30];
    char                city[20];
    char                address[30];
    int                 birth_date[3];
    long                user_id;
    char                gender[6];
}Data;

typedef struct User{
    Account            *head;             //Lista racuna
    Data                user_data;        //Informacije o korisniku
}User;

typedef struct Tree{
    User                user;
    struct Tree        *left;
    struct Tree        *right;
}Tree;

Tree * new_leaf(User);
void insert_in_tree(Tree**, User);
Transaction * new_transaction(long, long, long, float, long, long);
void add_transaction(long, long, long, float,Transaction**, long , long);
Account * new_account(long, long, float, float, float);
void print_transactions(Transaction **);
void add_account(long, long, float, float, float, Account**);
void print_accounts(Account **);
void generate_user_id(User *);
long generate_account_id(User *);
void create_user();
void create_account(User *);
void create_transaction(Account *, Account *, float);
void add_money(Account *);
void print_add_money_menu();
void different_account_transfer(Account *);
Account * choose_account(Account *, long);
void print_main_menu();
void print_add_user();
void print_logged_user();
void search_user();
void search_user_rec(Tree**,char[20],char[30]);
void logged_menu();
void regulate_debt();
void take_debt();
void load_users();




//Binary Search Tree implementation

Tree *root = null;
Tree *trenutni_korisnik = null;

Tree * new_leaf(User user){
    Tree * new              = (Tree*)malloc(sizeof(Tree));

    new->left               = null;
    new->right              = null;
    new->user               = user;
    return new;
}

void insert_in_tree(Tree **root, User user){
    if(!*root){
        *root               = new_leaf(user);
    }
    else if(strcmp((*root)->user.user_data.name,user.user_data.name)>0)
        insert_in_tree(&(*root)->left,user);
    else if(strcmp((*root)->user.user_data.name,user.user_data.name)<0)
        insert_in_tree(&(*root)->right,user);
    else if(strcmp((*root)->user.user_data.name,user.user_data.name)==0){
        if(strcmp((*root)->user.user_data.surname,user.user_data.surname)>=0)
            insert_in_tree(&(*root)->left,user);
        else
            insert_in_tree(&(*root)->right,user);
    }
}

void print_users(Tree **root){
    if(*root) {
        print_users(&(*root)->left);
        printf("%s %s %ld", (*root)->user.user_data.name, (*root)->user.user_data.surname,
               (*root)->user.user_data.user_id);
        print_users(&(*root)->right);
    }
}

//Tranasction double linked list implementation

Transaction * new_transaction(long transaction_id, long first_account_id,
                              long second_account_id, float change,
                              long first_user_id, long second_user_id)
{
    Transaction * new = (Transaction*)malloc(sizeof(Transaction));
    new->previous           = null;
    new->next               = null;
    new->first_user_id      = first_user_id;
    new->second_user_id     = second_user_id;
    new->first_account_id   = first_account_id;
    new->second_account_id  = second_account_id;
    new->transaction_id     = transaction_id;
    new->change             = change;
    return new;

}

void add_transaction(long transaction_id, long first_account_id,
                     long second_account_id, float change, Transaction **head,
                     long first_user_id, long second_user_id)
{
    if(!*head) {
        *head = new_transaction(transaction_id, first_account_id, second_account_id, change,
                                first_user_id,second_user_id);
        return;
    }
    Transaction * temp = *head;
    while(temp->next){
        temp = temp->next;
    }
    temp->next = new_transaction(transaction_id,first_account_id,second_account_id,change,
                                 first_user_id,second_user_id);
    temp->next->previous = temp;

}

void print_transactions(Transaction **head){
    Transaction *temp = *head;
    while(temp){
        printf("***************************\n"
               "*Transaction ID: %ld      \n"
               "*First account ID: %ld    \n"
               "*Second account ID: %ld   \n"
               "*First user ID: %ld        \n"
               "*Second user ID: %ld       \n"
               "*Change: %f                \n"
               "***************************\n",
               temp->transaction_id,temp->first_account_id,temp->second_account_id,
               temp->first_user_id,temp->second_user_id,temp->change);



        temp = temp->next;
    }
}

//Account double linked list implementation

Account *chosen = null;

Account * new_account(long account_id, long user_id, float balance, float debt_release, float debt)
{
    Account* new            = (Account*)malloc(sizeof(Account));
    new->next               = null;
    new->previous           = null;
    new->account_id         = account_id;
    new->user_id            = user_id;
    new->balance            = balance;
    new->debt_release       = debt_release;
    new->debt               = debt;
    new->head               = null;
    return new;
}

void add_account(long account_id, long user_id, float balance, float debt_release,
                      float debt, Account **head)
{
    if(!*head){
        *head = new_account(account_id,user_id,balance,debt_release,debt);
        return;
    }
    Account * temp = *head;
    while(temp->next){
        temp = temp->next;
    }
    temp->next = new_account(account_id,user_id,balance,debt_release,debt);
    temp->next->previous = temp;
}

void print_accounts(Account **head){
    Account *temp = * head;
    while(temp){
        printf("***********************\n"
               "*Account ID: %ld\n"
               "*Balance : %.2f \n"
               "*Debt release: %.2f \n"
               "*Debt: %.2f \n"
               "***********************\n",temp->account_id,temp->balance,
                                           temp->debt_release,temp->debt);

        temp = temp->next;
    }
}

Account * choose_account(Account *head,long account_id){
    Account * temp = head;
    while(temp){
        if(temp->account_id == account_id)
            return temp;
        temp = temp->next;
    }
    printf("Nalog nije pronadjen, los broj naloga\n");
    return null;
}

//*********************************************************************************//

void generate_user_id(User * user){
    for(int i = 0; i < 20; i++){
        user->user_data.user_id += user->user_data.name[i];
    }
    for(int i = 0; i < 30; i++){
        user->user_data.user_id += user->user_data.surname[i];
    }
    for(int i = 0; i < 20; i++){
        user->user_data.user_id += user->user_data.city[i];
    }
    for(int i = 0; i < 30; i++){
        user->user_data.user_id += user->user_data.address[i];
    }
}

long generate_account_id(User * user){
    return user->user_data.user_id - 54321%(user->user_data.birth_date[0]) + rand();

}

void create_user(){
    Data data;
    print_add_user();
    printf("Unesite vase ime:\n");
    scanf("%s",data.name);
    printf("Unesite vase prezime:\n");
    scanf("%s",data.surname);
    printf("Unesite grad:\n");
    scanf("%s",data.city);
    printf("Unesite vasu adresu:\n");
    stdin = freopen(NULL,"r",stdin);
    scanf("%[^\n]s",data.address);
    stdin = freopen(NULL,"r",stdin);
    printf("Unesite vas datum rodjenja, unose se brojevi koje prati razmak, osim godine koju ne prati razmak\n");
    stdin = freopen(NULL,"r",stdin);
    scanf("%d %d %d",&data.birth_date[0],&data.birth_date[1],&data.birth_date[2]);
    printf("Unesite vas pol (Musko, Zensko) :\n");
    scanf("%s",data.gender);
    User user;
    user.user_data = data;
    user.head = null;
    generate_user_id(&user);

    char flag;
    printf("Da li zelite da otvorite racun? (Y/N) \n");
    stdin = freopen(NULL,"r",stdin);
    scanf("%c",&flag);
    if(flag=='Y' || flag == 'y')
        create_account(&user);

    insert_in_tree(&root,user);
    printf("Nalog uspesno otvoren!\n");
}

void create_account(User * user){
    add_account(generate_account_id(user),user->user_data.user_id,0.0,0.0,0.0,&user->head);

}

void create_transaction(Account * first_account, Account * second_account,float change){
    long transaction_id = (first_account->account_id + second_account->account_id + rand()) % 10 + rand();
    add_transaction(transaction_id,first_account->account_id,second_account->account_id,-change,
                    &first_account->head,first_account->user_id,second_account->user_id);
    add_transaction(transaction_id,second_account->account_id,first_account->account_id,change,
                    &second_account->head,second_account->user_id,first_account->user_id);
}

void transfer(Account * first_account, Account *second_account){
    float change = 0;
    char flag;
    repeat:
    printf("Koliko novca zelite da prebacite?\n");
        scanf("%f", &change);

    if(first_account->balance<change && first_account->debt_release + first_account->balance>change){
        first_account->balance+= first_account->debt_release;
        first_account->debt = first_account->debt_release;
        first_account->debt_release = 0;
    }
    else if(first_account->balance<change && first_account->debt_release + first_account->balance<change){
        printf("Nemate dovoljno sredstava za ovu transakciju.\n");
        printf("Da li zelite da uplatite jos sredstava? Y/N\n");
        stdin = freopen(NULL,"r",stdin);
        scanf("%c",&flag);
        if(flag=='y' || flag=='Y'){
            add_money(first_account);
            goto repeat;
        }
        else return;
    }
    else if(first_account->balance>change){
        first_account->balance-=change;
        if(change<=10000)
            first_account->debt_release+= change * 10/100;
        else if(change>=10000 && change<=100000)
            first_account->debt_release+= change * 20/100;
        else if(change>=100000)
            first_account->debt_release+= change * 40/100;
    }
    second_account->balance +=change;
    if(second_account->debt!=0 && second_account->balance!=0){
        if(second_account->balance>=second_account->debt){
            second_account->balance -= second_account->debt;
            second_account->debt = 0;
        }
        else{
            second_account->debt -= second_account->balance;
            second_account->balance = 0;
        }
    }
    create_transaction(first_account,second_account,change);
    printf("Uspesno ste prebacili novac\n");

}

void bank_transfer(Account * bankomat, Account * second_account,float change){
    second_account->balance +=change;
    second_account->debt_release += change * 12/100;
    if(second_account->debt!=0 && second_account->balance!=0){
        if(second_account->balance>=second_account->debt){
            second_account->balance -= second_account->debt;
            second_account->debt = 0;
        }
        else{
            second_account->debt -= second_account->balance;
            second_account->balance = 0;
        }
    }
    create_transaction(bankomat,second_account,change);
}

void print_add_money_menu(){
    printf("*********************************************\n"
           "**  Dobrodosli u meni za dodavanje novca   **\n"
           "*********************************************\n"
           "**-----------------------------------------**\n"
           "*********************************************\n"
           "**(1)Uplatite novac                        **\n"
           "**(2)Prebacite novac sa drugog racuna      **\n"
           "**(0)Izlaz iz menija                       **\n"
           "*********************************************\n");
}

void add_money(Account * account){
    int opcija;
    Account *bankomat = new_account(999999999,999999999,999999,0,0);
    do{
        print_add_money_menu();
        scanf("%d",&opcija);
        if(opcija==1){
            float novac = 0;
            printf("Koliko novca biste zeleli da uplatite?\n");
            scanf("%f",&novac);
            bank_transfer(bankomat,account,novac);
            printf("Novac uspesno uplacen");
            break;

        }
        if(opcija==2){
        different_account_transfer(account);
        break;
        }
    }
    while(opcija);
    free(bankomat);
}

void different_account_transfer(Account * account){
    Account * temp = account,*transfer_account,*count_temp;
    int account_count = 1;
    long account_id;
    while(temp->previous){
        temp = account->previous;
    }
    count_temp = temp;
    while(count_temp->next){
        count_temp = count_temp->next;
        account_count++;
    }
    if(account_count>1) {
        print_accounts(&temp);
        printf("Molimo vas odaberite nalog sa kog zelite da prenesete novac, iskopirajte Account ID\n");
        stdin = freopen(null,"r",stdin);
        scanf("%ld", &account_id);
        do {
            transfer_account = choose_account(temp, account_id);
        } while (transfer_account == null);
        transfer(transfer_account, account);
    }
    else{
        printf("Imate samo jedan nalog, molimo vas uplatite novac\n");
    }
    temp = null;
    free(temp);
    count_temp = null;
    free(count_temp);


}

void search_user(){
    char name[20],surname[30];
    printf("Unesite ime:\n");
    scanf("%s",name);
    printf("Unesite prezime:\n");
    scanf("%s",surname);

    search_user_rec(&root,name,surname);
    if(trenutni_korisnik!=null)
        printf("Uspesno ulogovani\n");
    else
        printf("Korisnik ne postoji\n");
}

void search_user_rec(Tree **root,char name[20],char surname[30]){
    if(*root){
        search_user_rec(&(*root)->left,name,surname);
        if(strcmp((*root)->user.user_data.name,name)==0
           && strcmp((*root)->user.user_data.surname,surname)==0)
            trenutni_korisnik = (*root);
        search_user_rec(&(*root)->right,name,surname);
    }
}

void total_balance(Account *head){
    Account *temp = head;
    int account_count = 0;
    float total = 0;
    while(temp){
        total += temp->balance;
        account_count++;
        temp = temp->next;
    }
    if(account_count>1)
        printf("Imate %.2f dinara raspodeljenih na %d racuna.\n",total,account_count);
    else
        printf("Imate %.2f dinara.\n",total);
}

void total_debt(Account *head){
    Account *temp = head;
    float total_debt = 0;
    int account_count = 0;
    while(temp){
        total_debt += temp->debt;
        account_count++;
        temp = temp->next;
    }
    if(account_count>1)
        printf("Dugujete %.2f dinara na %d racuna\n",total_debt,account_count);
    else
        printf("Dugujete %.2f dinara\n",total_debt);

}

void print_main_menu(){
    printf("_________________________________________________\n"
           "|***********************************************|\n"
           "|**********Chikkin Banking Sistem 3000**********|\n"
           "|***********************************************|\n"
           "|*****************Dobrodosli********************|\n"
           "|***********************************************|\n"
           "|**(1)           Nov Korisnik                 **|\n"
           "|***********************************************|\n"
           "|**(2)            Imam Nalog                  **|\n"
           "|***********************************************|\n"
           "|**(0)              Izadji                    **|\n"
           "|***********************************************|\n"
           "|***********************************************|\n"
           "-------------------------------------------------\n");
}

void print_add_user(){
    printf("***************************************\n"
           "**    Meni za dodavanje korisnika    **\n"
           "***************************************\n");
}

void print_logged_user(){
    Data data = trenutni_korisnik->user.user_data;
    if(trenutni_korisnik!=null) {
        printf("*********************************************\n"
               "**            User logged now is           **\n"
               "*********************************************\n"
               "** Ime : %s\n"
               "** Prezime: %s\n"
               "** Grad : %s\n"
               "** Adresa : %s\n"
               "** Datum rodjenja %d %d %d\n"
               "** Pol : %s\n"
               "*********************************************\n", data.name, data.surname, data.city,
               data.address, data.birth_date[0], data.birth_date[1], data.birth_date[2], data.gender);
    }
}

void print_account_options(){
    printf("*********************************************\n"
           "**         Opcije vaseg/ih racuna          **\n"
           "*********************************************\n"
           "**(1) Prikazite sve racune                 **\n"
           "**(2) Otvorite nov racun                   **\n"
           "**(3) Ukupan balans                        **\n"
           "**(4) Ukupno dugovanje                     **\n"
           "*********************************************\n"
           "**(5) Odaberite pojedinacni racun          **\n"
           "*********************************************\n"
           "**(6) Dodajte novac na racun               **\n"
           "**(7) Prikazite transakcije racuna         **\n"
           "**(8) Regulisite dugovanja za ovaj racun   **\n"
           "**(9) Uzmite pozajmicu                     **\n"
           "*********************************************\n"
           "**(0) Nazad                                **\n"
           "*********************************************\n");
}

void main_menu(){
    int choice;
    do{
        print_main_menu();
        scanf("%d",&choice);
        if(choice==1){
            create_user();
        }
        if(choice==2){
            search_user();
            logged_menu();
            break;
        }
        if(choice==0){
            exit(0);
        }

    }while(choice!=0);
}

void logged_menu(){
    int choice;
    do{
        print_logged_user();
        print_account_options();
        scanf("%d",&choice);
        if(choice==1){
            print_accounts(&trenutni_korisnik->user.head);
        }
        if(choice==2){
            create_account(&trenutni_korisnik->user);
            printf("Nalog uspesno napravljen!\n");
        }
        if(choice==3){
            total_balance(trenutni_korisnik->user.head);
        }
        if(choice==4){
            total_debt(trenutni_korisnik->user.head);
        }
        if(choice==5){
            long id;
            print_accounts(&trenutni_korisnik->user.head);
            printf("Unesite ID racuna, iskopirajte ga odozgo.\n");
            scanf("%ld",&id);
            chosen = choose_account(trenutni_korisnik->user.head,id);
        }
        if(choice==6){
            if(chosen==null) {
                printf("Molimo vas prvo odaberite racun.\n");
                continue;
            }
            add_money(chosen);
        }
        if(choice==7){
            if(chosen==null){
                printf("Molimo vas prvo odaberite racun.\n");
                continue;
            }
            print_transactions(&trenutni_korisnik->user.head->head);
        }
        if(choice==8){
            if(chosen==null){
                printf("Molimo vas prvo odaberite racun.\n");
                continue;
            }
            regulate_debt();
        }
        if(choice==9){
            if(chosen==null){
                printf("Molimo vas prvo odaberite racun.\n");
                continue;
            }
            take_debt();
        }
        if(choice==0){
            main_menu();
        }
    }while(choice);
}

void regulate_debt(){
    Account * account = chosen;
    char flag;
    printf("Da li ste sigurni da hocete da regulisete dugove za nalog ID: %ld?\n",account->account_id);
    stdin = freopen(null,"r",stdin);
    scanf("%c",&flag);
    if(flag=='y' || flag=='Y') {
        if (account->debt != 0 && account->balance != 0) {
            if (account->balance >= account->debt) {
                account->balance -= account->debt;
                account->debt = 0;
            } else {
                account->debt -= account->balance;
                account->balance = 0;
            }
        }
    }
    else
        return;
}

void take_debt(){
    float temp = 0;
    do{
        printf("Mozete uzeti pozajamicu od %.2f dinara\n"
               "Koliku pozajmicu biste zeleli da uzmete?\n",chosen->debt_release);
        scanf("%f",&temp);
    }while(temp<=chosen->debt_release);
    chosen->debt_release -=temp;
    chosen->debt += temp;
    chosen->balance += temp;
}

void load_users(){
    FILE *file = fopen("DB_USERS.txt","r");
    //TODO

}

int main() {
    srand(time(NULL));
    main_menu();



    return 0;
}