/*******************************************************
Litra Light Handler
(C)2022 - Chris Jones (@IPv6freely)
********************************************************/


#include <string.h>
#include <stdlib.h>
#include <hidapi.h>

#define MAX_STR 255

void usage(){
    printf("USAGE:\n");
    printf("./litra <serial_number> <on|off>\tTurns on Light with serial number <serial_number>\n");
    printf("./litra list\t\t\t\tLists all USB devices connected\n");
}

int main(int argc, char* argv[])
{

    int res;
    unsigned char light_on[20] = {0x11,0xff,0x04,0x1c,0x01};
    unsigned char light_off[20] = {0x11,0xff,0x04,0x1c};
    wchar_t wstr[MAX_STR];
    wchar_t serial_number[MAX_STR];
    char mfgr[MAX_STR];
    hid_device *handle;
    int i;

    if ( argc == 2 && strcmp(argv[1],"list") == 0 ){

        printf("Getting all connected USB devices...\n");

        struct hid_device_info *devs, *cur_dev;

        devs = hid_enumerate(0x0, 0x0);
        cur_dev = devs;
        while (cur_dev) {

            memset(mfgr,0,sizeof(mfgr));

            wcstombs(mfgr, cur_dev->manufacturer_string, MAX_STR);

            if  ( strcmp(mfgr,"Logi" ) == 0 ) {
                printf("%ls | %ls | vid:pid %04hx:%04hx | serial_number %ls\n", 
                    cur_dev->manufacturer_string, 
                    cur_dev->product_string, 
                    cur_dev->vendor_id, 
                    cur_dev->product_id, 
                    cur_dev->serial_number);
            }
            cur_dev = cur_dev->next;
        }
        hid_free_enumeration(devs);

    }

    else if (argc == 3) {    

        res = hid_init();

        mbstowcs(serial_number, argv[1], MAX_STR);

        handle = hid_open(0x046d, 0xc900, serial_number);

        if (!handle) {
            wprintf(L"Open failed on serial number %ls", serial_number);
            return 1;
        }
        else {

            if ( strcmp(argv[2],"on") == 0 ) {
                res = hid_write(handle,light_on,20);
            }
            else if ( strcmp(argv[2],"off") == 0 ) {
                res = hid_write(handle,light_off,20);

            }
            else {
                usage();
                return 1;
            }

            if (!res) { 
                printf("FAILED.\n");
            }
            else {
                printf("SUCCESS! Wrote %d bytes.\n",20);
                res = hid_exit();
            }

            hid_close(handle);

        }
    }
    else {
        usage();
    }

    return 0;

}
