#!/bin/bash
git config --global --unset user.name
git config --global --unset user.email
git config --unset user.name
git config --unset user.email
echo "Global and local git user unset"