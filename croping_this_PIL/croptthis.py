#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Jul 29 12:00:20 2021

@author: honepa
"""

from PIL import ImageOps, Image

img = Image.open("1.jpg")

border = (0, 150, 0, 100)

crop_img = ImageOps.crop(img, border)

crop_img.save("1_test_crop.jpg", "JPEG")

for i in range(75):
    if i != 0:
        img = Image.open(str(i)+".jpg")
        crop_img = ImageOps.crop(img, border)
        crop_img.save(str(i) + "_crop.jpg", "JPEG")