#include <string>
#include <iostream>

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

        // Parse exactly 4 octets
        for (int octetNum = 0; octetNum < 4; octetNum++)
        {
            // Must start with a digit
            if (pos >= str.length() || str[pos] < '0' || str[pos] > '9')
            {
                valid = false;
                break;
            }

            int value = 0;
            int digits = 0;

            // Parse digits manually
            while (pos < str.length() &&
                   str[pos] >= '0' && str[pos] <= '9')
            {
                value = value * 10 + (str[pos] - '0');
                digits++;

                // More than 3 digits can't be a valid IPv4 octet
                if (digits > 3)
                {
                    valid = false;
                    break;
                }

                pos++;
            }

            if (!valid || value > 255)
            {
                valid = false;
                break;
            }

            // Add this octet to the 32-bit address
            address = (address << 8) | value;

            // The first three octets must be followed by '.'
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

        // Check for optional port
        int port = -1;

        if (pos < str.length() && str[pos] == ':')
        {
            pos++;

            // A colon must be followed by at least one digit
            if (pos >= str.length() ||
                str[pos] < '0' || str[pos] > '9')
            {
                continue;
            }

            int portValue = 0;
            int portDigits = 0;

            while (pos < str.length() &&
                   str[pos] >= '0' && str[pos] <= '9')
            {
                portValue = portValue * 10 + (str[pos] - '0');
                portDigits++;

                // Port can't have more than 5 digits
                if (portDigits > 5)
                {
                    valid = false;
                    break;
                }

                pos++;
            }

            if (!valid || portValue > 65535)
            {
                continue;
            }

            port = portValue;
        }

        // We found a valid IP (with or without port)
        outAddress = address;
        outPort = port;

        return true;
    }

    return false;
}



int main(int argc, char* argv[])
{
	unsigned long address;
	int port;
	std::string input;

	std::cout << "Enter a string (or 'END' to quit): ";
	while(std::getline(std::cin, input) && input != "END") {

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