#include <string>
#include <iostream>

/*
This function was written using generative AI.
Model & Version: GPT-5.6 - Luna
Date: 9/26/26
Tested: Yes
Understood: Yes I commented blocks to show my understanding
*/
bool extractIPv4(const std::string& str, unsigned long& outAddress, int& outPort)
{
    // Default failure values
    outAddress = 0;
    outPort = -1;

    // Check every character as a possible beginning of an IP
    for (size_t i = 0; i < str.length(); i++)
    {
        size_t pos = i;
        unsigned long address = 0;
        bool valid = true;

        //Loop for 4 octets
        for (int octetNum = 0; octetNum < 4; octetNum++)
        {
            // Check if the start of the octet is a number
            if (pos >= str.length() || str[pos] < '0' || str[pos] > '9')
            {
                valid = false;
                break;
            }

            int value = 0;
            int digits = 0;

            // Start parsing each digit if position is less than string length and it's a valid number
            while (pos < str.length() &&
                   str[pos] >= '0' && str[pos] <= '9')
            {
				//Calculates the value of the address and increases the digits
                value = value * 10 + (str[pos] - '0');
                digits++;

                // Checks if there are more than 3 digits in the octet
                if (digits > 3)
                {
                    valid = false;
                    break;
                }

                pos++;
            }
			// Checks if the octet has a valid value
            if (!valid || value > 255)
            {
                valid = false;
                break;
            }

            // Add the octet to the 32-bit address
            address = (address << 8) | value;

            // Check if every octet apart from the 4th one is separated by a '.'
            if (octetNum < 3)
            {
                if (pos >= str.length() || str[pos] != '.')
                {
                    valid = false;
                    break;
                }

                pos++;
            }
        }

        if (!valid)
        {
            continue;
        }

        // Check for port number
        int port = -1;

        if (pos < str.length() && str[pos] == ':')
        {
            pos++;

            // Check if the port starts with a valid digit
            if (pos >= str.length() ||
                str[pos] < '0' || str[pos] > '9')
            {
                continue;
            }

            int portValue = 0;
            int portDigits = 0;

			//Loop through all the port digits
            while (pos < str.length() &&
                   str[pos] >= '0' && str[pos] <= '9')
            {
				//Calculates the value of the port and increases the digits
                portValue = portValue * 10 + (str[pos] - '0');
                portDigits++;

                //Checks if the port has more than 5 digits
                if (portDigits > 5)
                {
                    valid = false;
                    break;
                }

                pos++;
            }

			//Checks if the port is a valid value
            if (!valid || portValue > 65535)
            {
                continue;
            }

            port = portValue;
        }

        //Assign the values if found and return true
        outAddress = address;
        outPort = port;

        return true;
    }

    return false;
}

/*
This function was partly written using generative AI.
Model & Version: GPT-5.6 - Luna
Date: 9/26/26
Lines: 155-160
Tested: Yes
Understood: Yes I commented blocks to show my understanding
*/

int main(int argc, char* argv[])
{
	unsigned long address;
	int port;
	std::string input;

	std::cout << "Enter a string (or 'END' to quit): ";
	while(std::getline(std::cin, input) && input != "END") {

		//In order to output the octets in the address, you must bishift to each octet and bitwise and with all 1's to get just that octet printed
		if (extractIPv4(input, address, port))
		{
			std::cout << "Extracted IPv4 address: " << ((address >> 24) & 0xFF) << "."
					<< ((address >> 16) & 0xFF) << "."
					<< ((address >> 8) & 0xFF) << "."
					<< (address & 0xFF);

			std::cout << " (decimal value: " << address;

			if (port != -1)
			{
				std::cout << ", port: " << port << ")";
			}
			else {
				std::cout << ", port: none)";
			}

			std::cout << std::endl;
		}
		else {
			std::cout << "Invalid input: no valid IPv4 address found" << std::endl;
		}
		std::cout << "Enter a string (or 'END' to quit): ";
	}
	return 1;
}
