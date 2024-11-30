module;

export module health_component;

export struct HealthComponent {
  int healthPercentage;


  explicit HealthComponent(int healthPercentage = 0) :
    healthPercentage(healthPercentage) {
  }
};
