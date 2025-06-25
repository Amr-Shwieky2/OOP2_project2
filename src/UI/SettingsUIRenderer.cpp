#include "../../include/UI/SettingsUIRenderer.h"
#include <iostream>
#include <cmath>

SettingsUIRenderer::SettingsUIRenderer(const sf::Font& font) : m_font(font) {
    std::cout << "Creating SettingsUIRenderer with Dependency Injection pattern..." << std::endl;
}

void SettingsUIRenderer::initializeTexts() {
    std::cout << "Initializing UI texts..." << std::endl;

    // Setup title text with hardcoded English
    setupTextProperties(m_titleText,
        "Settings",  // نص ثابت
        m_config.titleSize,
        m_config.titleColor);
    m_titleText.setPosition(570, 70);
    applyTextStyling(m_titleText, sf::Text::Italic);

    // Setup instruction text  
    setupTextProperties(m_backInstructionText,
        "Press ESC to go back",  // نص ثابت
        m_config.instructionSize,
        m_config.instructionColor);
    m_backInstructionText.setPosition(570, 550);
    applyTextStyling(m_backInstructionText, sf::Text::Italic);

    std::cout << "UI texts initialized" << std::endl;
}

void SettingsUIRenderer::updateAnimation(float deltaTime) {
    // Update animation timer with configurable speed
    m_animationTime += deltaTime * m_animationSpeed;

    // Update individual animation effects
    if (m_glowEnabled) {
        updateTitleGlowEffect();
    }

    // Future: Add more animation effects here
    // - Text color transitions
    // - Position animations
    // - Scale effects
}

void SettingsUIRenderer::updateTitleGlowEffect() {
    float glowIntensity = calculateGlowIntensity();
    sf::Color glowColor = calculateGlowColor(glowIntensity);
    m_titleText.setFillColor(glowColor);
}

float SettingsUIRenderer::calculateGlowIntensity() const {
    // Use sine wave to create smooth oscillation
    float sineValue = std::sin(m_animationTime * m_config.glowFrequency);

    // Map sine wave (-1 to 1) to desired range (0.7 to 1.0)
    float normalizedSine = (sineValue + 1.0f) * 0.5f; // Now 0 to 1
    return m_config.glowIntensityMin +
        (normalizedSine * (m_config.glowIntensityMax - m_config.glowIntensityMin));
}

sf::Color SettingsUIRenderer::calculateGlowColor(float intensity) const {
    sf::Color baseColor = m_config.titleColor;
    baseColor.a = static_cast<sf::Uint8>(255 * intensity);
    return baseColor;
}

void SettingsUIRenderer::renderBackground(sf::RenderWindow& window, const sf::Sprite& background) {
    window.draw(background);
}

void SettingsUIRenderer::renderTexts(sf::RenderWindow& window) {
    // Render title with shadow effect
    if (m_shadowEnabled) {
        sf::Text titleShadow = m_titleText;
        createShadowEffect(titleShadow, m_titleText, m_config.shadowOffset);
        window.draw(titleShadow);
    }

    // Render main title text
    window.draw(m_titleText);

    // Render instruction text (no shadow needed for smaller text)
    window.draw(m_backInstructionText);
}

void SettingsUIRenderer::createShadowEffect(sf::Text& shadowText, const sf::Text& originalText, sf::Vector2f offset) {
    // Configure shadow appearance
    shadowText.setFillColor(m_config.shadowColor);

    // Position shadow with offset
    sf::Vector2f originalPos = originalText.getPosition();
    shadowText.setPosition(originalPos.x + offset.x, originalPos.y + offset.y);
}

void SettingsUIRenderer::renderAnimationEffects(sf::RenderWindow& window) {}

void SettingsUIRenderer::setTitlePosition(float x, float y) {
    m_titleText.setPosition(x, y);
    std::cout << "Title position updated to (" << x << ", " << y << ")" << std::endl;
}

void SettingsUIRenderer::setInstructionPosition(float x, float y) {
    m_backInstructionText.setPosition(x, y);
    std::cout << "Instruction position updated to (" << x << ", " << y << ")" << std::endl;
}

void SettingsUIRenderer::setupTextProperties(sf::Text& text, const std::string& content, int size, sf::Color color) {
    text.setFont(m_font);
    text.setString(content);
    text.setCharacterSize(size);
    text.setFillColor(color);
}

void SettingsUIRenderer::applyTextStyling(sf::Text& text, sf::Text::Style style) {
    text.setStyle(style);
}