
# this for mastering oriented object programming

import os as os

print(os.getcwd())

class SportTeam:
    total_nb_of_team = 0
    penality_amount = 200
    def __init__(self, name: str, wins: int, losses: int):
        self.name = name
        self.wins = wins
        self.losses = losses
        self.penality = 0
        
        if self.__class__=="BaskeballTeam":
            BasketballTeam.total_nb_of_team += 1
        else:
            FootballTeam.total_nb_of_team+=1

    @staticmethod
    def pluralize(total, singular, plural=None):
        assert isinstance(total, int) and total >= 0, " sorry we need a positive int"
        if plural is None:
            string = singular+'s'
            plural = singular if total == 1 else string
            return f"{total} {plural}"
        else:
            string = singular if total == 1 else plural
            return f"{total} {string}"


    def TeamPenality(self):
        return f"{self.name} penality is {self.penality+self.penality_amount}"

    @classmethod
    def set_Penality_amount(cls, amount):
        cls.penality_amount = amount

    @classmethod
    def from_string(cls, stat_as_string):
        name, wins, losses = stat_as_string.split('-')
        return cls(name, int(wins), int(losses))

    @classmethod
    def from_file(cls, stat_as_file):
        with open(stat_as_file, 'r') as f:
            name, wins, losses = f.readlines().strip().split("-")
        return cls(name, int(wins), int(losses))

    def stats(self):
        return f"Statistics for {self.name}:{self.pluralize(self.wins,'win')} and {self.pluralize(self.losses,'loss','losses')}"

class BasketballTeam(SportTeam):
    """This is the class that shows statistics about Basketball teams"""
    def stats(self):
        return f'[BasketbalTeam] {super().stats()}'
    
class FootballTeam(SportTeam):
    def stats(self):
        return f'[FootbalTeam] {super().stats()}' #{super().pluralize(self.nulls,'null') }'

#======================================================================================
from selenium import webdriver
from selenium.webdriver.common.keys import Keys
from time import sleep, strftime
import credentials
from webdriver_manager.firefox import GeckoDriverManager


class InstragramBot:
    def __init__(self,username,password):
        self.username = username
        self.password = password
        self.bot = webdriver.Firefox(executable_path=GeckoDriverManager().install())
    
    def login(self): 
        bot=self.bot
        bot.get('https://www.instagram.com/accounts/login/')
        sleep(3)
        email=bot.find_element_by_name("username").send_keys(self.username)
        Password=bot.find_element_by_name("password").send_keys(self.password)
        sleep(1)
        bot.find_elements_by_name("password").send_keys(Keys.RETURN)
        sleep(3)

    def findMyfollowers( self, number_of_followers):
        bot=self.bot
        bot.get('htpps://instagram.com/'+self.username)
        sleep(2)
        bot.find_elements_by_xpath('//a[@href=/'+self.username+'/followers/').click()
        sleep(2)
        popu=bot.find_element_class_name('isgrP')
        followers=[]
        i=1
        while len(followers)<= number_of_followers:
            bot.execute_script('argments[0].scrollTop=arguments[0].scrollHeight',popu)
            sleep(0.5)
            follow=bot.find_element_by_class_name('FPmhx')

insta=InstragramBot('louise92_2.0','135642')
insta.login()
insta.findMyfollowers(2)


if __name__=='__main__':
    
    print(os.getcwd())
    #print(help(BasketBallTeam))
    with open("/simple_projects/data.txt",'r') as file:
        name,wins,losses=file.readline().strip().split("-")
        team1=FootballTeam("name",int(wins),int(losses))

    team2=BasketballTeam.from_string('azsnn-15-25')
    print(team2.stats())
    BasketballTeam.set_Penality_amount(5555)
    print(team1.stats())