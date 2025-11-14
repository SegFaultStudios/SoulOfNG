#include "Widgets/MultiplayerMenuConnectionDialogWidget.hpp"

#include "AssetsManager.hpp"
#include <iostream>

MultiplayerMenuConnectionDialogWidget::MultiplayerMenuConnectionDialogWidget(const std::string& name) : UIDialogWidget(name), m_text("Trying to connect to the server")
{
    setTitle("Connect to server");

    const sf::Vector2f buttonSize{5.0f, 2.5f};

    m_connectButton = std::make_unique<UIButton>("Connect");
    m_connectButton->setText("Connect");
    m_connectButton->setPosition(getPosition() + sf::Vector2f(50, 150));
    m_connectButton->setSize(buttonSize);
    m_connectButton->setOnClick([this]
    {
        if(m_isTryingToConnect)
            return;

        m_isTryingToConnect = true;
        m_rotatingImage->startSpinning();
        m_text.setText("Trying to connect to the server");
        m_text.setVisible(true);

        m_connectingFuture = std::async([this]{return tryToConnect();});
    });

    m_cancelButton = std::make_unique<UIButton>("Cancel");

    m_cancelButton->setText("Cancel");
    m_cancelButton->setPosition(getPosition() + sf::Vector2f(250, 150));
    m_cancelButton->setSize(buttonSize);

    m_cancelButton->setOnClick([this]
    {
        m_doNotTryToConnect = true;

        if(m_connectingFuture.valid())
            m_connectingFuture.wait(); //Kinda bad...

        this->setResult(DialogResult::Rejected);
        this->close();
    });

    auto texture = AssetsManager::instance().loadTexture("./resources/textures/refresh.png");

    m_rotatingImage = std::make_unique<RotatingImage>("", texture);

    m_rotatingImage->setPosition(getPosition() + sf::Vector2f(getSize().x / 2.0f, 75.0f));
    m_rotatingImage->setSize({0.1, 0.1});
    m_rotatingImage->stopSpinning();

    m_text.setPosition(getPosition() + sf::Vector2f(getSize().x / 2.0f - 110.0f, 110.0f));
    m_text.setCharacterSize(15);
    m_text.setVisible(false);
}

void MultiplayerMenuConnectionDialogWidget::update(float deltaTime)
{
    UIDialogWidget::update(deltaTime);

    m_rotatingImage->update(deltaTime);

    if(m_connectingFuture.valid())
    {
        std::future_status status = m_connectingFuture.wait_for(std::chrono::milliseconds(10));

        if(status == std::future_status::ready)
        {
            m_isTryingToConnect = false;

            bool result = m_connectingFuture.get();

            if(result)
            {
                std::cout << "Connected" << std::endl;

                this->setResult(DialogResult::Accepted);

                this->close();
            }
            else
            {
                m_text.setText("Failed to connect to the server");
                m_rotatingImage->stopSpinning();
            }
        }
    }
}

void MultiplayerMenuConnectionDialogWidget::setPosition(const sf::Vector2f& position)
{
    UIDialogWidget::setPosition(position);
    m_connectButton->setPosition(position + sf::Vector2f(50.0f, 150.0f));
    m_cancelButton->setPosition(position + sf::Vector2f(250.0f, 150.0f));
    m_rotatingImage->setPosition(position + sf::Vector2f(getSize().x / 2.0f, 75.0f));
    m_text.setPosition(position + sf::Vector2f(getSize().x / 2.0f - 110.0f, 110.0f));
}

bool MultiplayerMenuConnectionDialogWidget::tryToConnect()
{
    sf::IpAddress remoteAddress(sf::IpAddress::LocalHost); 
    unsigned short remotePort = 9090;
    sf::Time timeout = sf::seconds(5.0f);
    int tries = 0;

    static constexpr unsigned short MAX_TRIES = 5;

    while(tries < MAX_TRIES && !m_doNotTryToConnect)
    {
        sf::Socket::Status status = m_socket.connect(remoteAddress, remotePort, timeout);

        if(status == sf::Socket::Status::Done)
            return true;

        ++tries;

        //SFML timeout does not work for some reason
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return false;
}

void MultiplayerMenuConnectionDialogWidget::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    UIDialogWidget::draw(target, states);

    if(!isOpen())
        return;

    target.draw(*m_cancelButton.get(), states);
    target.draw(*m_connectButton.get(), states);

    m_rotatingImage->draw(target, states);
    m_text.draw(target, states);
}

void MultiplayerMenuConnectionDialogWidget::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    UIDialogWidget::handleEvent(event, window);

    if(!isOpen())
        return;

    m_cancelButton->handleEvent(event, window);
    m_connectButton->handleEvent(event, window);
}