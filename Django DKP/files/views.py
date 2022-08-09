from django.http.response import Http404
from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.shortcuts import redirect
from django.views import generic
from django.forms import formset_factory
from players.models import Player, Item, File
from .forms import *
from files.parseFile import *
import logging
from django.conf import settings


#Landing page, displays upload file option. File List not used(yet?)
def index(request):
    file_list = File.objects.order_by('fileDate')
    context = {'file_list': file_list}
    return render(request, 'files/index.html', context)

#Gives the user the option to upload different log types, and parse them accordingly.
def uploadFile(request):
    choice = 0
    logging.debug("UploadFile View")
    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        logging.debug("Form??")
        logging.debug(request.POST)
        if form.is_valid():
            type = form.cleaned_data.get("type")
            #type corresponds to the 1st part of the logTypes tuple in forms
            context = []
            if type[0] == "1": #
                choice = 1
                parseGuild(request)
            elif type[0] == "2":
                choice = 2
                parseRaid(request)
            elif type[0] == "3":
                logging.debug("LogFile Type: Wins")
                choice = 3
                winsList = parseWins(request)
                editWinsFormSet = formset_factory(editWins)
                dataForm = editWinsFormSet(initial=winsList)
                request.session['winsList'] = winsList
                #context = {'winsList': winsList}
                #dataForm = editWins()
                #dataForm = editWins(request.POST, request.FILES)
                #dataForm = editWins(winsList)
                logging.debug("Going into the return")
            return render(request, 'files/confirm.html', {'dataForm': dataForm, 'winsList': winsList})
    else:
        form = UploadFileForm()

    if choice == 3:
        logging.debug("INSIDE APPLY")
        
        #ADD CONFIRMATION?

    return render(request, 'files/uploadFile.html', {'form': form})

def confirm(request):
    logging.debug("INSIDE CONFIRM")
    result = request.POST
    logging.debug(request.POST.get('winsList'))
    applyWins(request)
    return render(request, 'files/confirm.html')