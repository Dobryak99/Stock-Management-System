#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <mysql.h>

void Log_in_menu();
void Main_menu();
void Show_all_assets(MYSQL * con, MYSQL_ROW row, MYSQL_RES * res);
void Buy_new_assets(MYSQL * con, MYSQL_RES * res);
void Buy_existing_assets(MYSQL * con, MYSQL_RES * res);
void Sell_assets(MYSQL * con, MYSQL_RES * res);

int main()
{
    const char * server = "localhost";              //using local server
    const char * database = "stocks";
    std::string username;
    std::string password;
    bool IsConnected = false;
    int choice;
    
    MYSQL * connection;
    MYSQL_RES * result;
    MYSQL_ROW row;
    
    while(choice != 2)
    {
        Log_in_menu();
        std::cin >> choice;
        while(std::cin.get() != '\n')
            continue;
        switch(choice)
        {
            case 1: std::cout << "\t\t\tEnter username: ";
                std::getline(std::cin, username);
                std::cout << "\t\t\tEnter Password: ";
                std::getline(std::cin, password);
                choice = 0;
                while(!IsConnected)
                {
                    connection = mysql_init(NULL);                  //initialize connection
                    if(!mysql_real_connect(connection, server, username.c_str(), password.c_str(), database, 0, NULL, 0))
                    {
                        system("clear");                        //clear terminal window
                        std::cout << "********************************************************\n"
                        << "***************Stock Management System******************\n"
                        << "********************************************************\n\n\n";
                        std::cout << "\n\t\t\tWrong input.\n\n\n";
                        std::cout << "\t\t\t1.Try again.\n";
                        std::cout << "\t\t\t2.Return.\n\n\n";
                        std::cout << "\t\t\tChoice: ";
                        std::cin >> choice;
                        while(std::cin.get() != '\n')
                            continue;
                        if(choice == 1)
                        {
                            std::cout << "\n\t\t\tEnter username: ";
                            std::getline(std::cin, username);
                            std::cout << "\t\t\tEnter Password: ";
                            std::getline(std::cin, password);
                            choice = 0;
                        }else{
                            choice = 0;
                            break;
                        }
                    }else{
                        IsConnected = true;
                    }
                }
                if(IsConnected)
                {
                    while(choice != 5)
                    {
                        Main_menu();
                        choice = 0;
                        std::cin >> choice;
                        while(std::cin.get() != '\n')
                            continue;
                        
                        switch(choice)
                        {
                            case 1:
                                {
                                    choice = 0;
                                    Show_all_assets(connection, row, result);
                                    break;
                                }
                                
                            case 2:
                                {
                                    choice = 0;
                                    Buy_new_assets(connection, result);
                                    break;
                                }
                                
                            case 3:
                                {
                                    choice = 0;
                                    Buy_existing_assets(connection, result);
                                    break;
                                }
                            
                            case 4:
                                {
                                    choice = 0;
                                    Sell_assets(connection, result);
                                    break;
                                }
                                
                            case 5:
                                {
                                    IsConnected = false;
                                    mysql_close(connection);
                                    break;
                                }
                                
                        }
                    }
                }
                break;
                
            case 2: std::cout << "Bye!\n";
                break;
        }
    }
    return 0;
}


void Log_in_menu()
{
    system("clear");                        //clear terminal window
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    
    std::cout << "\t\t\t1. Log in.\n";
    std::cout << "\t\t\t2. Exit.\n";
    
    std::cout << "\n\n\n\t\t\tChoice: ";
}

void Main_menu()
{
    system("clear");                        //clear terminal window
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    
    std::cout << "\t\t\t1.Show all assets.\n";
    std::cout << "\t\t\t2.Buy new asset.\n";
    std::cout << "\t\t\t3.Buy existing assets.\n";
    std::cout << "\t\t\t4.Sell.\n";
    std::cout << "\t\t\t5.Quit.\n";
    
    std::cout << "\n\n\n\t\t\tChoice: ";
}

void Show_all_assets(MYSQL * con, MYSQL_ROW row, MYSQL_RES * res)
{
    system("clear");                        //clear terminal window
    int choice;
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    mysql_query(con, "SELECT * FROM stocks");
    res = mysql_use_result(con);
    std::cout << "|" << std::setw(10) << std::left << "ID" << "|"
    << std::setw(10) << std::left << "NAME" << "|"
    << std::setw(10) << std::left << "AMOUNT" << "|"
    << std::setw(10) << std::left << "PRICE" << "|" << std::endl;
    
    while(row = mysql_fetch_row(res))
    {
        std::cout << "---------------------------------------------" << std::endl;
        for(int i = 0; i < mysql_num_fields(res); i++)
        {
            std::cout << "|" << std::setw(10) << std::left << row[i];
        }
        std::cout << "|\n";
    }
    while(choice != 1)
    {
        std::cout << "\n\n\t\t\t1. Return.\n";
        std::cout << "\n\n\t\t\tChoice: ";
        std::cin >> choice;
    }
    while(std::cin.get() != '\n')
        continue;
    mysql_free_result(res);
}

void Buy_new_assets(MYSQL * con, MYSQL_RES * res)
{
    system("clear");
    int choice;
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    std::cout << "\t\tOpeartion of buying new assets.\n\n";
    std::cout << "\t\t\tCompany: ";
    std::string comp_name;
    std::getline(std::cin, comp_name);
    std::cout << "\t\t\tAmount of shares: ";
    int amount;
    std::cin >> amount;
    std::cout << "\t\t\tPrice: ";
    double price;
    std::cin >> price;
    while(std::cin.get() != '\n')
        continue;
    std::stringstream ss;
    ss << "INSERT stocks(NAME, AMOUNT, PRICE) VALUES(\""
    << comp_name.c_str() << "\", " << amount << ", " << price << ");";
    std::string res_str = ss.str();
    mysql_query(con, res_str.c_str());
    res = mysql_use_result(con);
    while(choice != 1)
    {
        system("clear");
        std::cout << "**************************************************\n"
        << "************Stock Management System***************\n"
        << "**************************************************\n\n\n";
        std::cout << "\t\tOrder completed!\n\n";
        std::cout << "\t\t\t1.Return\n";
        std::cout << "\n\n\n\t\t\tChoice: ";
        std::cin >> choice;
    }
    mysql_free_result(res);
}

void Buy_existing_assets(MYSQL * con, MYSQL_RES * res)
{
    system("clear");                        //clear terminal window
    int choice;
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    std::cout << "\t\tOpeartion of buying existing assets.\n\n";
    std::cout << "\t\t\tCompany: ";
    std::string comp_name;
    std::getline(std::cin, comp_name);
    std::cout << "\t\t\tAmount of shares: ";
    int amount;
    std::cin >> amount;
    std::cout << "\t\t\tPrice: ";
    double price;
    std::cin >> price;
    while(std::cin.get() != '\n')
        continue;
    std::stringstream ss;
    ss << "UPDATE stocks SET Price = (Amount * Price + "
    << amount * price << ") / (Amount + " << amount
    << "), Amount = Amount + " << amount << " WHERE Name = \"" << comp_name.c_str() << "\";";
    std::string res_str = ss.str();
    mysql_query(con, res_str.c_str());
    res = mysql_use_result(con);
    while(choice != 1)
    {
        system("clear");
        std::cout << "**************************************************\n"
        << "************Stock Management System***************\n"
        << "**************************************************\n\n\n";
        std::cout << "\t\tOrder completed!\n\n";
        std::cout << "\t\t\t1.Return\n";
        std::cout << "\n\n\n\t\t\tChoice: ";
        std::cin >> choice;
    }
    mysql_free_result(res);
}

void Sell_assets(MYSQL * con, MYSQL_RES * res)
{
    system("clear");                        //clear terminal window
    int choice;
    std::cout << "********************************************************\n"
    << "***************Stock Management System******************\n"
    << "********************************************************\n\n\n";
    std::cout << "\t\t\t!!!WARNING!!!\n\n";
    std::cout << "This operation sell all of the shares of picked company!\n";
    std::cout << "\n\t\t\tCompany: ";
    std::string comp_name;
    std::getline(std::cin, comp_name);
    std::stringstream ss;
    ss << "DELETE FROM stocks WHERE Name = \"" << comp_name.c_str() << "\";";
    std::string res_str = ss.str();
    mysql_query(con, res_str.c_str());
    res = mysql_use_result(con);
    while(choice != 1)
    {
        system("clear");
        std::cout << "**************************************************\n"
        << "************Stock Management System***************\n"
        << "**************************************************\n\n\n";
        std::cout << "\t\tOrder completed!\n\n";
        std::cout << "\t\t\t1.Return\n";
        std::cout << "\n\n\n\t\t\tChoice: ";
        std::cin >> choice;
    }
    mysql_free_result(res);
}
