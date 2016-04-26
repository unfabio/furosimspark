#
# prepare soccer simulation
#

# setup soccer specific materials
# run "scripts/rcs-materials.rb"
run "scripts/rcs-materials-textures.rb"

importBundle "soccer"

# soccer namespace
$soccerNameSpace = "Soccer"

# register a variable in the soccer namespace
def addSoccerVar(name, value)
   createVariable($soccerNameSpace, name, value)
end

# helper to get the value of a variable in the soccer namespace
def getSoccerVar(name)
  eval <<-EOS
    #{$soccerNameSpace}.#{name}
  EOS
end

# set a random seed (a seed of 0 means: use a random random seed)
randomServer = get($serverPath+'random')
if (randomServer != nil)
  randomServer.seed(0)
end

# the soccer field dimensions in meters
#addSoccerVar('FieldLength', randomServer.uniformRND(100.0,110.9).floor().to_f())
#addSoccerVar('FieldWidth', randomServer.uniformRND(64.0,75.9).floor().to_f())
# Joschka: settings for Atlanta
#addSoccerVar('FieldLength', 45.0)
#addSoccerVar('FieldWidth', 30.0)

#Jan: alternative settings for Atlanta
addSoccerVar('FieldLength', 50.0)
addSoccerVar('FieldWidth', 32.0)
addSoccerVar('FieldHeight', 40.0)
#addSoccerVar('GoalWidth', 7.32)
addSoccerVar('GoalWidth', 9.0)
addSoccerVar('GoalDepth', 2.0)
#addSoccerVar('GoalHeight', 2.44) # FIFA: 2.44
addSoccerVar('GoalHeight', 1.0)
addSoccerVar('PenaltyLength',8)
addSoccerVar('PenaltyWidth',10.5)
addSoccerVar('BorderSize', 10.0)
addSoccerVar('FreeKickDistance', 4.5)
addSoccerVar('FreeKickMoveDist', 9.0)
addSoccerVar('AutomaticKickOff', true)
addSoccerVar('WaitBeforeKickOff', 8.0)

# agent parameters
addSoccerVar('AgentMass', 75.0)
addSoccerVar('AgentRadius',  3.5)
addSoccerVar('AgentMaxSpeed', 10.0)

# ball parameters
addSoccerVar('BallRadius', 0.21)
#addSoccerVar('BallDensity',2)
addSoccerVar('BallMass',0.46) # FIFA: 410-450 g

# soccer rule parameters
addSoccerVar('RuleGoalPauseTime',3.0)
addSoccerVar('RuleKickInPauseTime',1.0)
addSoccerVar('RuleHalfTime',8.0 * 60)
addSoccerVar('RuleDropBallTime',30)
addSoccerVar('SingleHalfTime', true)
addSoccerVar('UseOffside',false)

# recorders
addSoccerVar('BallRecorder',"Ball/geometry/recorder")
addSoccerVar('LeftGoalRecorder',"LeftGoalSpace/GoalBoxL/GoalColliderL/recorder")
addSoccerVar('RightGoalRecorder',"RightGoalSpace/GoalBoxR/GoalColliderR/recorder")

# the soccer field dimensions in meters
addSoccerVar('GoalKickDist', 1.0)

# soccer game settings
addSoccerVar('CoinTossForKickOff', false)
addSoccerVar('PenaltyShootout', false)

addSoccerVar('AutomaticQuit', false)
addSoccerVar('ChangeSidesInSecondHalf', false)

# agent parameters
addSoccerVar('MaxRobotTypeCount', 7)
addSoccerVar('MinRobotTypesCount', 3)
addSoccerVar('MaxSumTwoRobotTypes', 9)

# Noise added to requested beam positions
addSoccerVar('BeamNoiseXY',0.05)
addSoccerVar('BeamNoiseAngle',10.0)
# charging foul parameters
addSoccerVar('UseCharging', false)
addSoccerVar('ChargingMinSpeed', 0.2)
addSoccerVar('ChargingMinBallDist', 0.2)
addSoccerVar('IllegalInterceptMinAngle', 70)
addSoccerVar('MaxTouchGroupSize', 2)

# auto ref parameters  FCP 2010
addSoccerVar('NotStandingMaxTime',30)
addSoccerVar('GoalieNotStandingMaxTime',60)
addSoccerVar('GroundMaxTime',15)
addSoccerVar('GoalieGroundMaxTime', 30)
addSoccerVar('MaxPlayersInsideOwnArea',3)
addSoccerVar('MinOppDistance',0.8)
addSoccerVar('Min2PlDistance',0.4)
addSoccerVar('Min3PlDistance',1.0)

# 2014 server changes
addSoccerVar('ReportScore', true)
addSoccerVar('LabelMessages', true)

# textures
createVariable('Nao', 'UseTexture', 'true')

scene = get($scenePath)
if (scene != nil)
  scene.importScene('rsg/agent/soccer.rsg')
end

# setup the GameControlServer
gameControlServer = get($serverPath+'gamecontrol')
if (gameControlServer != nil)
  gameControlServer.initControlAspect('GameStateAspect')
  gameControlServer.initControlAspect('BallStateAspect')
  gameControlServer.initControlAspect('SoccerRuleAspect')
end

# init monitorItems to transmit game state information
monitorServer = get($serverPath+'monitor')
if (monitorServer != nil)
  monitorServer.registerMonitorItem('GameStateItem')
end

# install the TrainerCommandParser to parse commands received from a
# monitor client
sparkRegisterMonitorCmdParser 'TrainerCommandParser'
