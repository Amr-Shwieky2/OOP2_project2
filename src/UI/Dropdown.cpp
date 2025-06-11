#include "Dropdown.h"
#include <iostream>

Dropdown::Dropdown(sf::Vector2f position, sf::Vector2f size, const sf::Font& font)
    : m_position(position), m_size(size), m_font(font) {

    try {
        m_background.setPosition(position);
        m_background.setSize(size);
        m_background.setFillColor(m_backgroundColor);
        m_background.setOutlineThickness(2);
        m_background.setOutlineColor(sf::Color(100, 100, 100));

        m_selectedText.setFont(font);
        m_selectedText.setCharacterSize(20);
        m_selectedText.setFillColor(m_textColor);
        m_selectedText.setPosition(position.x + 10, position.y + 5);

        m_arrowText.setFont(font);
        m_arrowText.setString("▼");
        m_arrowText.setCharacterSize(16);
        m_arrowText.setFillColor(m_textColor);
        m_arrowText.setPosition(position.x + size.x - 25, position.y + 8);

        std::cout << "Dropdown created successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error creating dropdown: " << e.what() << std::endl;
    }
}

void Dropdown::addOption(const std::u8string& text, int value) {
    try {
        std::cout << "Adding dropdown option with value: " << value << std::endl;

        // ← إضافة تحقق أمان للنص
        if (text.empty()) {
            std::cout << "Warning: Empty text for dropdown option" << std::endl;
            m_options.push_back({ u8"Empty Option", value });
        }
        else {
            m_options.push_back({ text, value });
        }

        if (m_selectedIndex == -1) {
            setSelectedIndex(0);
        }

        std::cout << "Option added successfully. Total options: " << m_options.size() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error adding option: " << e.what() << std::endl;
        // ← إضافة خيار آمن في حالة الخطأ
        m_options.push_back({ u8"Safe Option", value });
    }
}

void Dropdown::setSelectedIndex(int index) {
    try {
        // ← تحقق محسن من صحة الفهرس
        if (m_options.empty()) {
            std::cout << "Cannot set index: no options available" << std::endl;
            m_selectedIndex = -1;
            return;
        }

        if (index >= 0 && index < static_cast<int>(m_options.size())) {
            int oldIndex = m_selectedIndex;
            m_selectedIndex = index;

            // ← تحقق أمان قبل تحديث النص
            if (updateSelectedTextSafely()) {
                std::cout << "Dropdown selection changed from " << oldIndex << " to " << index << std::endl;

                if (m_onSelectionChanged && oldIndex != index) {
                    try {
                        m_onSelectionChanged(index, m_options[index].value);
                    }
                    catch (const std::exception& e) {
                        std::cout << "Error in selection callback: " << e.what() << std::endl;
                    }
                }
            }
            else {
                std::cout << "Failed to update selected text, reverting index" << std::endl;
                m_selectedIndex = oldIndex;
            }
        }
        else {
            std::cout << "Invalid dropdown index: " << index << " (valid range: 0-" << (m_options.size() - 1) << ")" << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Critical error setting dropdown selection: " << e.what() << std::endl;
        m_selectedIndex = -1;
    }
}

int Dropdown::getSelectedValue() const {
    try {
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_options.size())) {
            return m_options[m_selectedIndex].value;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error getting selected value: " << e.what() << std::endl;
    }
    return -1;
}

bool Dropdown::handleMouseMove(sf::Vector2f mousePos) {
    try {
        if (m_isOpen) {
            m_hoveredIndex = -1;
            for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
                if (getOptionBounds(i).contains(mousePos)) {
                    m_hoveredIndex = i;
                    break;
                }
            }
            return true;
        }
        else {
            return getMainBounds().contains(mousePos);
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error in dropdown mouse move: " << e.what() << std::endl;
        return false;
    }
}

bool Dropdown::handleMousePressed(sf::Vector2f mousePos) {
    try {
        if (m_isOpen) {
            if (m_hoveredIndex >= 0 && m_hoveredIndex < static_cast<int>(m_options.size())) {
                std::cout << "Dropdown option clicked: " << m_hoveredIndex << std::endl;
                setSelectedIndex(m_hoveredIndex);
                m_isOpen = false;
                return true;
            }
            else {
                std::cout << "Dropdown closed by clicking outside" << std::endl;
                m_isOpen = false;
                return false;
            }
        }
        else {
            if (getMainBounds().contains(mousePos)) {
                std::cout << "Dropdown opened" << std::endl;
                m_isOpen = true;
                return true;
            }
        }
        return false;
    }
    catch (const std::exception& e) {
        std::cout << "Error in dropdown mouse press: " << e.what() << std::endl;
        return false;
    }
}

void Dropdown::update(float deltaTime) {
    // Animation could be added here
}

void Dropdown::render(sf::RenderWindow& window) {
    try {
        // Draw main dropdown
        window.draw(m_background);
        window.draw(m_selectedText);
        window.draw(m_arrowText);

        // Draw options if open
        if (m_isOpen && !m_options.empty()) {
            for (int i = 0; i < static_cast<int>(m_options.size()); ++i) {
                try {
                    sf::RectangleShape optionBg;
                    optionBg.setPosition(m_position.x, m_position.y + m_size.y + (i * 30));
                    optionBg.setSize(sf::Vector2f(m_size.x, 30));

                    if (i == m_hoveredIndex) {
                        optionBg.setFillColor(m_highlightColor);
                    }
                    else {
                        optionBg.setFillColor(m_backgroundColor);
                    }
                    optionBg.setOutlineThickness(1);
                    optionBg.setOutlineColor(sf::Color(100, 100, 100));

                    window.draw(optionBg);

                    // ← رسم النص بطريقة آمنة
                    if (renderOptionTextSafely(window, i)) {
                        // نجح الرسم
                    }
                    else {
                        // فشل الرسم - ارسم نص بديل
                        sf::Text fallbackText;
                        fallbackText.setFont(m_font);
                        fallbackText.setString("Option " + std::to_string(i));
                        fallbackText.setCharacterSize(20);
                        fallbackText.setFillColor(m_textColor);
                        fallbackText.setPosition(m_position.x + 10, m_position.y + m_size.y + (i * 30) + 5);
                        window.draw(fallbackText);
                    }
                }
                catch (const std::exception& e) {
                    std::cout << "Error rendering option " << i << ": " << e.what() << std::endl;
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cout << "Critical error rendering dropdown: " << e.what() << std::endl;
    }
}

void Dropdown::setColors(sf::Color background, sf::Color text, sf::Color highlight) {
    try {
        m_backgroundColor = background;
        m_textColor = text;
        m_highlightColor = highlight;

        m_background.setFillColor(m_backgroundColor);
        m_selectedText.setFillColor(m_textColor);
        m_arrowText.setFillColor(m_textColor);
    }
    catch (const std::exception& e) {
        std::cout << "Error setting dropdown colors: " << e.what() << std::endl;
    }
}

// ← دالة آمنة لتحديث النص المختار
bool Dropdown::updateSelectedTextSafely() {
    try {
        if (m_selectedIndex >= 0 && m_selectedIndex < static_cast<int>(m_options.size())) {
            const auto& selectedOption = m_options[m_selectedIndex];

            // تحقق من أن النص ليس فارغاً
            if (selectedOption.text.empty()) {
                m_selectedText.setString("Empty");
                std::cout << "Warning: Selected option has empty text" << std::endl;
                return true;
            }

            // محاولة تحويل النص بطريقة آمنة
            try {
                sf::String selectedString = sf::String::fromUtf8(
                    selectedOption.text.begin(),
                    selectedOption.text.end()
                );
                m_selectedText.setString(selectedString);
                std::cout << "Updated selected text (index: " << m_selectedIndex << ")" << std::endl;
                return true;
            }
            catch (const std::exception& utf8_error) {
                std::cout << "UTF-8 conversion error: " << utf8_error.what() << std::endl;
                // استخدم نص بديل
                m_selectedText.setString("Option " + std::to_string(m_selectedIndex));
                return true;
            }
        }
        else {
            std::cout << "Invalid selected index for text update" << std::endl;
            m_selectedText.setString("No Selection");
            return false;
        }
    }
    catch (const std::exception& e) {
        std::cout << "Critical error updating selected text: " << e.what() << std::endl;
        m_selectedText.setString("Error");
        return false;
    }
}

// ← دالة آمنة لرسم نص الخيارات
bool Dropdown::renderOptionTextSafely(sf::RenderWindow& window, int index) {
    try {
        if (index < 0 || index >= static_cast<int>(m_options.size())) {
            return false;
        }

        const auto& option = m_options[index];
        sf::Text optionText;
        optionText.setFont(m_font);

        // تحويل آمن للنص
        sf::String optionString;
        if (!option.text.empty()) {
            try {
                optionString = sf::String::fromUtf8(option.text.begin(), option.text.end());
            }
            catch (const std::exception& e) {
                std::cout << "UTF-8 error for option " << index << ": " << e.what() << std::endl;
                optionString = "Option " + std::to_string(index);
            }
        }
        else {
            optionString = "Empty";
        }

        optionText.setString(optionString);
        optionText.setCharacterSize(20);
        optionText.setFillColor(m_textColor);
        optionText.setPosition(m_position.x + 10, m_position.y + m_size.y + (index * 30) + 5);

        window.draw(optionText);
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "Error rendering option text " << index << ": " << e.what() << std::endl;
        return false;
    }
}

// ← دالة محدثة للحصول على حدود الخيار
sf::FloatRect Dropdown::getOptionBounds(int index) const {
    try {
        if (index >= 0 && index < static_cast<int>(m_options.size())) {
            return sf::FloatRect(
                m_position.x,
                m_position.y + m_size.y + (index * 30),
                m_size.x,
                30
            );
        }
    }
    catch (const std::exception& e) {
        std::cout << "Error getting option bounds: " << e.what() << std::endl;
    }
    // إرجاع مستطيل فارغ في حالة الخطأ
    return sf::FloatRect(0, 0, 0, 0);
}

sf::FloatRect Dropdown::getMainBounds() const {
    return sf::FloatRect(m_position.x, m_position.y, m_size.x, m_size.y);
}