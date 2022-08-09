# Generated by Django 4.0.1 on 2022-01-24 20:08

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('players', '0003_alter_file_filedate_alter_item_itemdate'),
    ]

    operations = [
        migrations.AlterField(
            model_name='file',
            name='fileDate',
            field=models.DateField(default=datetime.datetime(2022, 1, 24, 14, 8, 50, 229552), verbose_name='Date'),
        ),
        migrations.AlterField(
            model_name='item',
            name='itemDate',
            field=models.DateField(default=datetime.datetime(2022, 1, 24, 14, 8, 50, 229552), verbose_name='Date'),
        ),
        migrations.AlterField(
            model_name='item',
            name='winner',
            field=models.CharField(default='NA', max_length=20, verbose_name='Winner'),
        ),
    ]
