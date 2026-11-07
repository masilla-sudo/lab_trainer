mkdir -p docs
cat > docs/lab9_scenario.md << 'EOF'
# Демонстрационный сценарий ЛР 9

1. Собрать проект: `cmake --build build`
2. Запустить: `./build/dispatcher_trainer`
3. Выполнить команды:
   - `help` — справка
   - `status` — текущие параметры
   - `set V-101 60` — изменить параметр (variant: Ok)
   - `set V-101 79` — предупреждение (variant: Warning)
   - `set V-101 100` — ошибка (variant: Error)
   - `find V-101` — поиск через optional
   - `find unknown` — nullopt
   - `stats` — статистика событий
   - `save` — сохранение журнала через filesystem
4. Ожидаемый результат: журнал сохранён в `logs/dispatch_session.txt`
EOF