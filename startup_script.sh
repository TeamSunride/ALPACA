#!/bin/bash

#start docker containers
sudo docker compose -f /home/sunride/ALPACA/docker-compose.yml up -d

#wait for influxdb to be healthy
echo "waiting for influxdb..."
until docker exec influxdb curl -sf --max-time 3 http://localhost:8086/health > /dev/null; do
	sleep 2
done
echo "influxdb ready!"


#wait for telegraf to be running
echo "waiting for telegraf..."
until docker ps --filter "name=telegraf" --filter "status=running" | grep -q telegraf; do
	sleep 2
done
echo "telegraf ready!"

sleep 1

echo "starting scripts"
xfce4-terminal --hold --command='/bin/bash -c "source /home/sunride/ALPACA/.venv/bin/activate && python3 /home/sunride/ALPACA/python_scripts/switchSending.py"' &
xfce4-terminal --hold --command='/bin/bash -c "source /home/sunride/ALPACA/.venv/bin/activate && python3 /home/sunride/ALPACA/python_scripts/timestamper.py"' &
echo "scripts started"

sleep 1

setsid -f chromium --new-instance --new-window 'http://localhost:3001/dashboards' >/dev/null 2>&1 < /dev/null

