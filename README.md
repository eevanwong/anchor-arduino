# Code for Anchor's Arduino w/ PN532 + ESP32 modules

Ensure you're using SeeedStudio's PN532 module library (https://github.com/Seeed-Studio/PN532) + Rocket's Low Power Library (https://github.com/rocketscream/Low-Power)


## Testing

**ESP32 Service**
- Replace the HTTP address the POST requests are made to in Arduino (i.e. `192.168...`).
- Check the Host Machine's IP Address
   - Open a Terminal and run: `ifconfig`
   - Look for the inet address under the active network interface (e.g., wlan0 or en0).
- Initialize the network SSID and password values.