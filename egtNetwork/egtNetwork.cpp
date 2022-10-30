/**
 * @file meritechIPegt.cpp
 * @author Owen O'Hehir
 * @brief EGT based GUI to display network stats
 * @version 0.1
 * @date 2022-10-29
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <ifaddrs.h>
#include <linux/if_link.h> //rtnl_link_stats
#include <egt/ui>

using namespace std;
using namespace egt;

#define PARAM_NUM 8

/* Change below to match the adapters on your device*/
#define ETHERNET_LABEL "enx000ec6cf6511"
#define WIFI_LABEL "wlp58s0"

int populate(const std::vector<std::shared_ptr<egt::Label>>);

int populate(const std::vector<std::shared_ptr<egt::Label>> labelVector){
    struct ifaddrs *ifaddr, *ifa;
    int column;
    char host[NI_MAXHOST];
    char netmask[NI_MAXHOST];

    // Default to down, no addresses, no flag incase where interface is not present
    for (column = 0; column < 2; column++){
        labelVector[0 + (column * PARAM_NUM)]->text("xx.xx.xx.xx");
        labelVector[1 + (column * PARAM_NUM)]->text("xx.xx.xx.xx");
        labelVector[2 + (column * PARAM_NUM)]->text("");
        labelVector[3 + (column * PARAM_NUM)]->text("DOWN");
    }
    
    if (getifaddrs(&ifaddr) == -1)
    {
        perror("getifaddrs");
        return(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;

        getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
        getnameinfo(ifa->ifa_netmask, sizeof(struct sockaddr_in), netmask, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

        if(strcmp(ifa->ifa_name,"eth0")==0 || strcmp(ifa->ifa_name, ETHERNET_LABEL)==0)
        	column = 0;
        else if (strcmp(ifa->ifa_name,"wlan0")==0 || strcmp(ifa->ifa_name, WIFI_LABEL)==0)
        	column = 1;
        else
        	continue;
        
        string s;
        if (ifa->ifa_flags & IFF_UP)
            s = "UP";
        else
            s = "DOWN";
        if (ifa->ifa_flags & IFF_RUNNING)
            s = s + ", RUNNING";
        labelVector[3 + (column * PARAM_NUM)]->text(s);
        // Only first 16bits are meaningful
        labelVector[2 + (column * PARAM_NUM)]->text(to_string(static_cast<unsigned short>(ifa->ifa_flags)));
        
        if (ifa->ifa_addr->sa_family == AF_INET){
            labelVector[0 + (column * PARAM_NUM)]->text(host);
            labelVector[1 + (column * PARAM_NUM)]->text(netmask);
        }else if (ifa->ifa_addr->sa_family == AF_PACKET && ifa->ifa_data != NULL){
            struct rtnl_link_stats *stats = static_cast<rtnl_link_stats*>(ifa->ifa_data);
            labelVector[4 + (column * PARAM_NUM)]->text(to_string(stats->tx_packets));
            labelVector[5 + (column * PARAM_NUM)]->text(to_string(stats->tx_bytes));
            labelVector[6 + (column * PARAM_NUM)]->text(to_string(stats->rx_packets));
            labelVector[7 + (column * PARAM_NUM)]->text(to_string(stats->rx_bytes));
        }
    }

    freeifaddrs(ifaddr);
    return(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    
    const char *gridLabels[PARAM_NUM] = {"IP Address", "Netmask", "Flags", "Status", "Tx Packets", "Tx Bytes", "Rx Packets", "Rx Bytes"};
    const char *gridTitles[] = {"Parameter", "Ethernet", "WiFi"};
    egt::Application app(argc, argv);
    egt::TopWindow window;
    auto grid = std::make_shared<egt::StaticGrid>(egt::StaticGrid::GridSize(3, 9));
    grid->margin(10);
    grid->horizontal_space(0);
    grid->vertical_space(1);
    window.add(expand(grid));

    auto exitButton = std::make_shared<egt::Button>("Exit");
    exitButton->align(AlignFlag::center_horizontal | AlignFlag::bottom);
    exitButton->margin(2);
    window.add(exitButton);

    exitButton->on_click([](Event &){
        exit(EXIT_SUCCESS);
    });

    // Titles
    for (auto i = 0; i < 3; i++){
        auto l = std::make_shared<egt::Label>(gridTitles[i]);
        grid->add(l,i,0);
        l->align(AlignFlag::left);
    }

    // Parameters
    for (auto i = 0; i < PARAM_NUM; i++){
        auto l = make_shared<egt::Label>(gridLabels[i]);
        grid->add(l, 0, i+1);
        l->align(AlignFlag::left);
    }

    // Populate rest of labels
    std::vector<std::shared_ptr<egt::Label>> labelVector;
    for (auto col = 1; col < 3; col++){
        for (auto row = 1; row < PARAM_NUM + 1; row++){
            auto l = std::make_shared<egt::Label>("..............................");
            grid->add(l, col, row);
            l->align(AlignFlag::left);
            l->text_align(AlignFlag::left);
            labelVector.push_back(l);
        }
    }
    
    populate(labelVector);
    
    egt::PeriodicTimer refreshTimer(std::chrono::seconds(3));
    refreshTimer.on_timeout([labelVector](){
      populate(labelVector);
    });

    refreshTimer.start();
    
    window.show();
    return app.run();
}
