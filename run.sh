#!/usr/bin/env bash

docker-compose up --detach
docker-compose exec ping bash
