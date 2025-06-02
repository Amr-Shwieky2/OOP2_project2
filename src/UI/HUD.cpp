#include "HUD.h"

HUD::HUD() {
}

HUD::~HUD() {
}

void HUD::setFont(const sf::Font& font) {
    m_scoreText.setFont(font);
    m_livesText.setFont(font);
}

void HUD::setCharacterSize(unsigned int size) {
    m_scoreText.setCharacterSize(size);
    m_livesText.setCharacterSize(size);
}

void HUD::setScore(int score) {
    m_score = score;
    m_scoreText.setString("Score: " + std::to_string(m_score));
}

void HUD::setLives(int lives) {
    m_lives = lives;
    m_livesText.setString("Lives: " + std::to_string(m_lives));
}

void HUD::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    (void)event; (void)window;
    // HUD is not interactive by default
}

void HUD::update(float deltaTime) {
    (void)deltaTime;
    if (!m_visible) return;

    // Position score in top-left corner of screen with some margin
    m_scoreText.setPosition(10.f, 10.f);

    // Position lives just below the score
    float offsetY = m_scoreText.getLocalBounds().height + 10.f;
    m_livesText.setPosition(10.f, 10.f + offsetY);
}

void HUD::render(sf::RenderTarget& target) {
    if (!m_visible) return;
    target.draw(m_scoreText);
    target.draw(m_livesText);
}
