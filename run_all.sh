#!/bin/bash

echo "Starting Automotive Network..."

./dashboard_receiver &

sleep 1

./speed_sender &
./temp_sender &
./battery_sender &

sleep 1

./dtc_reader &

echo ""
echo "All ECUs Started"
echo ""

wait
