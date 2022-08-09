from django.db import models
from datetime import *


class Player(models.Model):
    playerName = models.CharField('Name', blank=False, max_length=20)
    playerClass = models.CharField('Class', blank=False, max_length=20)
    playerType = models.CharField('Type', blank=False, max_length=20)
    currentDKP = models.IntegerField('DKP', default=0)
    monthPercent = models.DecimalField('30Day', default = 0.00, decimal_places=2, max_digits=5)
    lifePercent = models.DecimalField('Lifetime', default = 0.00, decimal_places=2, max_digits=5)
    checksAvail = models.IntegerField('Possible', default = 0)
    checksEarned = models.IntegerField('Earned', default = 0)
    def __str__(self):
        return self.playerName

class Item(models.Model):
    itemName = models.CharField('Name', blank=False, max_length=50)
    winner = models.CharField('Winner', default='NA', max_length=20)
    price = models.IntegerField(default=0)
    itemDate = models.DateField('Date', default=datetime.now())
    def __str__(self):
        return self.itemName

class File(models.Model):
    fileType = models.CharField('Type', blank=False, max_length=20)
    fileDate = models.DateField('Date', default=datetime.now())
    admin = models.CharField(blank=False, max_length=20)
    def __str__(self):
        return self.fileType + ' - ' + self.fileDate
