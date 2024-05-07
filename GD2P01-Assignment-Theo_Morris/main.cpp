#include <iostream>
#include <SFML/Graphics.hpp>

int main() {

	sf::RenderWindow window(sf::VideoMode(800, 600), "window");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            // Handle text input
            if (event.type == sf::Event::TextEntered) {
                //if (event.text.unicode == '\b') { // Backspace
                //    if (!inputBuffer.empty()) {
                //        inputBuffer.pop_back();
                //        inputText.setString(inputBuffer);
                //    }
                //}
                //else if (event.text.unicode < 128) { // Printable characters
                //    inputBuffer += static_cast<char>(event.text.unicode);
                //    inputText.setString(inputBuffer);
                //}
            }
            // Handle Enter key press
            if (event.type == sf::Event::KeyPressed) {
                //if (event.key.code == sf::Keyboard::Enter) {
                //    // Send message to server
                //    if (!inputBuffer.empty()) {
                //        // Send the message to the server
                //        send(clientSock, inputBuffer.c_str(), inputBuffer.length(), 0);
                //        // Clear the input buffer and text field
                //        inputBuffer.clear();
                //        inputText.setString("");
                //    }
                //}
            }
        }
        window.clear();

        // Draw text input field
        //window.draw(inputText);

        //window.draw(clientText);

        window.display();
    }
}